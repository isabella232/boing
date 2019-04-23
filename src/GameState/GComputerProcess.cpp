#include "GComputerProcess.h"

static const TInt SPEED = 4;

GComputerProcess::GComputerProcess(GGameState *aGameState, TUint32 aType) {
  mGameState = aGameState;
  mState = MOVE_STATE;
  mSprite = new BSprite(0, PLAYER_SLOT, IMG_PADDLE, aType);
  mSprite->w = 8;
  mSprite->h = 32;
  mSprite->flags |= SFLAG_RENDER;
  mSprite->cMask |= STYPE_EBULLET;
  mGameState->AddSprite(mSprite);
  Reset();
}

GComputerProcess::~GComputerProcess() {
  mSprite->Remove();
  delete mSprite;
}

void GComputerProcess::Reset() {
  if (mSprite->type & STYPE_PLAYER) {
    mSprite->x = 8;
  } else {
    mSprite->x = SCREEN_WIDTH - 16;
  }
  mSprite->y = (SCREEN_HEIGHT / 2) - 16;
}

void GComputerProcess::Pause(TBool aPause) {
  if (aPause) {
    mSprite->flags &= ~SFLAG_RENDER;
    mState = WAIT_STATE;
  } else {
    mSprite->flags |= SFLAG_RENDER;
    mState = MOVE_STATE;
  }
}

TBool GComputerProcess::WaitState() {
  return ETrue;
}

TBool GComputerProcess::MoveState() {
  if (mGameState->GameOver()) {
    return ETrue;
  }

  if (mSprite->cType) {
    mSprite->cType = 0;
  }

  BSprite *ball = mGameState->Ball();

  if (((mSprite->type & STYPE_PLAYER) && ball->vx < 0) || ((mSprite->type & STYPE_ENEMY) && ball->vx > 0)) {
    if (ball->y > (mSprite->y + 20)) {
      mSprite->y = mSprite->y + SPEED;
    } else if (ball->y < (mSprite->y + 12)) {
      mSprite->y = mSprite->y - SPEED;
    }

    if (mSprite->y < 0) {
      mSprite->y = 0;
    } else if (mSprite->y > (SCREEN_HEIGHT - 32)) {
      mSprite->y = SCREEN_HEIGHT - 32;
    }
  }

  return ETrue;
}

TBool GComputerProcess::RunBefore() {
  return ETrue;
}

TBool GComputerProcess::RunAfter() {
  switch (mState) {
    case MOVE_STATE:
      return MoveState();
    case WAIT_STATE:
      return WaitState();
    default:
      return EFalse;
  }
}
