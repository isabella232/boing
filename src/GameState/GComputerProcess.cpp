#include "GComputerProcess.h"

static const TInt SPEED = 4;

GComputerProcess::GComputerProcess(GGameState *aGameState, TUint32 aType) {
  mGameState = aGameState;
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
TBool GComputerProcess::RunBefore() {
  return ETrue;
}
TBool GComputerProcess::RunAfter() {
  if (!mGameState->GameOver()) {
    if (mSprite->cType) {
      mSprite->cType = 0;
    }

    if ((mSprite->type & STYPE_PLAYER && mGameState->Ball()->vx < 0) || (mSprite->type & STYPE_ENEMY && mGameState->Ball()->vx > 0)) {
      if (mGameState->Ball()->vy > 0 && mGameState->Ball()->y > mSprite->y + 16) {
        mSprite->y = mSprite->y + SPEED;
      } else if (mGameState->Ball()->vy < 0 && mGameState->Ball()->y < mSprite->y + 16) {
        mSprite->y = mSprite->y - SPEED;
      }
      if (mSprite->y < 0) {
        mSprite->y = 0;
      }
      if (mSprite->y > (SCREEN_HEIGHT - 32)) {
        mSprite->y = SCREEN_HEIGHT - 32;
      }
    }
  }
  return ETrue;
}
// void BBitmap::FillRect(BViewPort *aViewPort, TInt aX1, TInt aY1, TInt aX2, TInt aY2, TUint8 aColor) {
