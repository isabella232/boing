#include "GBallProcess.h"
#include <math.h>

#define PI TFloat(M_PI)

class BallSprite : public BSprite {
public:
  BallSprite(TInt aPri, TUint16 bm, TUint16 img = 0, TUint32 aType = STYPE_DEFAULT) : BSprite(aPri, bm, img, aType) {
    vx = 0;
    vy = 0;
  }

  void Collide(BSprite *aOther) {
    if (aOther->type == STYPE_PLAYER) {
      if (x < (aOther->x + aOther->w) - vx ) {
        vy = (y - aOther->y -16)/4;
        x = aOther->x + aOther->w;
        cType |= aOther->type;
      }
    } else if (aOther->type == STYPE_ENEMY) {
      if (x < aOther->x + vx ) {
        vy = (y - aOther->y -16)/4;
        x = aOther->x - 4;
        cType |= aOther->type;
      }
    }
    else {
      cType |= aOther->type;
    }
  };
};

GBallProcess::GBallProcess(GGameState *aGameState, TFloat aVelocity) {
  mGameState = aGameState;
  mVelocity = aVelocity;
  mState = WAIT_STATE;
  mSprite = new BallSprite(0, COMMON_SLOT, IMG_BALL, STYPE_PBULLET|STYPE_EBULLET);
  mSprite->cMask = STYPE_ENEMY | STYPE_PLAYER;
  mSprite->flags |= SFLAG_CHECK;
  mSprite->flags &= ~SFLAG_RENDER;
  mSprite->w = mSprite->h = 4;
  aGameState->AddSprite(mSprite);

  // Randomize direction
  if (Random(0, 1)) {
    mAngle = RIGHT_ANGLE;
  } else {
    mAngle = LEFT_ANGLE;
  }
}

GBallProcess::~GBallProcess() {
  mSprite->Remove();
  delete mSprite;
}

// velocity determines difficulty (speed of ball)
void GBallProcess::Reset(TFloat aVelocity) {
  mState = MOVE_STATE;
  mSprite->flags |= SFLAG_RENDER;

  mSprite->x = TFloat(SCREEN_WIDTH) / 2;
  mSprite->y = TFloat(SCREEN_HEIGHT) / 2;

  TFloat angle = Random(mAngle, mAngle + ANGLE_RANGE) * PI / 180;

  mSprite->vx = cos(angle) * aVelocity;
  mSprite->vy = sin(angle) * aVelocity;
  mSprite->flags |= SFLAG_RENDER;
#ifdef ENABLE_AUDIO
  gSoundPlayer.SfxNewBall();
#endif
}

void GBallProcess::SpeedUp() {
  mSprite->vx *= 1.01f;
  mSprite->vy *= 1.01f;
}

void GBallProcess::CheckCollision(BSprite *paddleSprite) {
  TBool overlapped = EFalse;
  TRect ballRect, paddleRect;

  for (TInt i = 0; i < 4; i++) {
    mSprite->x += mSprite->vx / 4;
    mSprite->GetRect(ballRect);
    paddleSprite->GetRect(paddleRect);

    if (ballRect.Overlaps(paddleRect)) {
      mSprite->Collide(paddleSprite);
      overlapped = ETrue;
      break;
    }
  }

  if (!overlapped) {
    mSprite->x -= mSprite->vx;
  }
}

TBool GBallProcess::RunBefore() {
  if (mState == WAIT_STATE) {
    return ETrue ;
  }

  const TFloat newX = mSprite->x + mSprite->vx,
               newY = mSprite->y + mSprite->vy;

  // Step through possible collision at high velocities
  if (mSprite->flags & SFLAG_CHECK) {
    if (newX <= 8 && mSprite->vx <= -8) {
      CheckCollision(mGameState->Player());
    } else if (newX >= SCREEN_WIDTH - 16 && mSprite->vx >= 8) {
      CheckCollision(mGameState->Computer());
    }
  }

  // Bounce of walls
  if (newY < 0) {
    SpeedUp();
    mSprite->y = 0;
    mSprite->vy = MAX(-MAX_VELOCITY, MIN(MAX_VELOCITY, -mSprite->vy));
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxBounceWall();
#endif
  } else if (newY > SCREEN_HEIGHT - 4) {
    SpeedUp();
    mSprite->y = SCREEN_HEIGHT - 4;
    mSprite->vy = MAX(-MAX_VELOCITY, MIN(MAX_VELOCITY, -mSprite->vy));
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxBounceWall();
#endif
  }

  return ETrue;
}

void GBallProcess::Pause(TBool aPause) {
  if (aPause) {
    mSprite->flags &= ~(SFLAG_RENDER | SFLAG_MOVE);
    mState = WAIT_STATE;
  } else {
    mSprite->flags |= SFLAG_RENDER | SFLAG_MOVE;
    mState = MOVE_STATE;
  }
}

TBool GBallProcess::WaitState() {
  return ETrue;
}

TBool GBallProcess::MoveState() {
  // Change X velocity on paddle collision
  if (mSprite->cType & STYPE_PLAYER) {
    SpeedUp();
    mSprite->vx = MAX(-MAX_VELOCITY, MIN(MAX_VELOCITY, -mSprite->vx));
    mSprite->cType &= ~STYPE_PLAYER;
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxBounceOffPlayer();
#endif
  } else if (mSprite->cType & STYPE_ENEMY) {
    SpeedUp();
    mSprite->vx = MAX(-MAX_VELOCITY, MIN(MAX_VELOCITY, -mSprite->vx));
    mSprite->cType &= ~STYPE_ENEMY;
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxBounceOffPlayer();
#endif
  }

  // Increment score and reset ball
  if (mSprite->x > SCREEN_WIDTH) {
    mState = WAIT_STATE;
    mAngle = LEFT_ANGLE;
    mGameState->PlayerScores();
  } else if (mSprite->x < 4) {
    mState = WAIT_STATE;
    mAngle = RIGHT_ANGLE;
    mGameState->ComputerScores();
  }

  return !mGameState->GameOver();
}

TBool GBallProcess::RunAfter() {
  switch (mState) {
    case MOVE_STATE:
      return MoveState();
    case WAIT_STATE:
      return WaitState();
    default:
      return EFalse;
  }
}
