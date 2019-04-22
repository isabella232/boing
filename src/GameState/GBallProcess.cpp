#include "GBallProcess.h"
#include <math.h>

#define PI TFloat(M_PI)

class BallSprite : public BSprite {
public:
  BallSprite(TInt aPri, TUint16 bm, TUint16 img = 0,
                             TUint32 aType = STYPE_DEFAULT) : BSprite(aPri, bm, img, aType){
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
  this->mGameState = aGameState;
  this->mVelocity = aVelocity;
  mSprite = new BallSprite(0, COMMON_SLOT, IMG_BALL, STYPE_PBULLET|STYPE_EBULLET);
  mSprite->cMask = STYPE_ENEMY | STYPE_PLAYER;
  mSprite->flags |= SFLAG_RENDER | SFLAG_CHECK;
  mSprite->w = mSprite->h = 4;
  aGameState->AddSprite(mSprite);

  // Randomize direction
  if (Random(0, 1)) {
    this->mAngle = RIGHT_ANGLE;
  } else {
    this->mAngle = LEFT_ANGLE;
  }

  Reset(this->mVelocity);
}

GBallProcess::~GBallProcess() {
  mSprite->Remove();
  delete mSprite;
}

// velocity determines difficulty (speed of ball)
void GBallProcess::Reset(TFloat aVelocity) {
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
  const TFloat newX = mSprite->x + mSprite->vx,
               newY = mSprite->y + mSprite->vy;

  if (newX <= 8) {
    CheckCollision(mGameState->Player());
  } else if (newX >= SCREEN_WIDTH - 16) {
    CheckCollision(mGameState->Computer());
  }

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

TBool GBallProcess::RunAfter() {
  if (mSprite->cType & STYPE_PLAYER) {
    SpeedUp();
    mSprite->vx = MAX(-MAX_VELOCITY, MIN(MAX_VELOCITY, -mSprite->vx));
    mSprite->cType &= ~STYPE_PLAYER;
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxBounceOffPlayer();
#endif
  }
  else if (mSprite->cType & STYPE_ENEMY) {
    SpeedUp();
    mSprite->vx = MAX(-MAX_VELOCITY, MIN(MAX_VELOCITY, -mSprite->vx));
    mSprite->cType &= ~STYPE_ENEMY;
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxBounceOffPlayer();
#endif
  }
  if (mSprite->x > SCREEN_WIDTH) {
    mGameState->PlayerScores();
    mAngle = LEFT_ANGLE;
    Reset(VELOCITY);
  } else if (mSprite->x < 4) {
    mGameState->ComputerScores();
    mAngle = RIGHT_ANGLE;
    Reset(VELOCITY);
  }

  if (mGameState->GameOver()) {
    return EFalse;
  }

  return ETrue;
}
