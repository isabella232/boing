#include "GPaddleProcess.h"

static const TInt DELTA_X = 4;

GPaddleProcess::GPaddleProcess(GGameState *aGameState) {
  mGameState = aGameState;
  mState = MOVE_STATE;
  mSprite = new BSprite(0, PLAYER_SLOT, IMG_PADDLE, STYPE_PLAYER);
  mSprite->w = 8;
  mSprite->h = 32;
  mSprite->flags |= SFLAG_RENDER | SFLAG_CHECK;
  mSprite->cMask |= STYPE_EBULLET;
  mGameState->AddSprite(mSprite);
  Reset();
}

GPaddleProcess::~GPaddleProcess() {
  mSprite->Remove();
  delete mSprite;
}

void GPaddleProcess::Reset() {
  mSprite->x = 8;
  mSprite->y = (SCREEN_HEIGHT / 2) - 16;
}

void GPaddleProcess::Pause(TBool aPause) {
  if (aPause) {
    mSprite->flags &= ~SFLAG_RENDER;
    mState = WAIT_STATE;
  } else {
    mSprite->flags |= SFLAG_RENDER;
    mState = MOVE_STATE;
  }
}

TBool GPaddleProcess::WaitState() {
  return ETrue;
}

TBool GPaddleProcess::MoveState() {
  if (mGameState->GameOver()) {
    return ETrue;
  }

  if (mSprite->cType) {
    mSprite->cType = 0;
  }

  if (gControls.IsPressed(JOYUP)) {
    mSprite->y = mSprite->y - DELTA_X;
    if (mSprite->y < 0) {
      mSprite->y = 0;
    }
  } else if (gControls.IsPressed(JOYDOWN)) {
    mSprite->y = mSprite->y + DELTA_X;
    if (mSprite->y > (SCREEN_HEIGHT - 32)) {
      mSprite->y = SCREEN_HEIGHT - 32;
    }
  }

  return ETrue;
}

TBool GPaddleProcess::RunBefore() {
  return ETrue;
}

TBool GPaddleProcess::RunAfter() {
  switch (mState) {
    case MOVE_STATE:
      return MoveState();
    case WAIT_STATE:
      return WaitState();
    default:
      return EFalse;
  }
}
