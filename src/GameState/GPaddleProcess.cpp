#include "GPaddleProcess.h"

static const TInt DELTA_X = 4;

GPaddleProcess::GPaddleProcess(GGameState *aGameState) {
  mGameState = aGameState;
  mSprite = new BSprite(0, PLAYER_SLOT, IMG_PADDLE, STYPE_PLAYER);
  mSprite->w = 8;
  mSprite->h = 32;
  mSprite->flags |= SFLAG_RENDER;
  mSprite->cMask |= STYPE_EBULLET;
  Reset();
  mGameState->AddSprite(mSprite);
}

GPaddleProcess::~GPaddleProcess() {
  mSprite->Remove();
  delete mSprite;
}

void GPaddleProcess::Reset() {
  mSprite->x = 8;
  mSprite->y = (SCREEN_HEIGHT / 2) - 16;
}
TBool GPaddleProcess::RunBefore() {
  return ETrue;
}
TBool GPaddleProcess::RunAfter() {
  if (!mGameState->GameOver()) {
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
  }
  return ETrue;
}
// void BBitmap::FillRect(BViewPort *aViewPort, TInt aX1, TInt aY1, TInt aX2, TInt aY2, TUint8 aColor) {
