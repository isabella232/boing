#include "Game.h"

#include "GGamePlayfield.h"
#include "GTitlePlayfield.h"
#include "GPaddleProcess.h"
#include "GComputerProcess.h"
#include "GBallProcess.h"
#include "GTitleProcess.h"

const TInt PLAYER_SCORE_X = 4;
const TInt PLAYER_SCORE_Y = 4;
const TInt COMPUTER_SCORE_X = SCREEN_WIDTH - 20;
const TInt COMPUTER_SCORE_Y = 4;
const TInt GAME_OVER_Y = SCREEN_HEIGHT / 2 - 8;

GGameState::GGameState(TBool aDemoMode) : BGameEngine(gViewPort) {
  mDemoMode = aDemoMode;
  mGameOver = EFalse;

  mFont8  = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_16x16);
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);

  gResourceManager.LoadBitmap(GAME_SPRITES_BMP, COMMON_SLOT, IMAGE_16x16);
  gResourceManager.LoadBitmap(PLAYER_SPRITES_BMP, PLAYER_SLOT, IMAGE_8x32);

  if (aDemoMode) {
    mPaddleProcess = ENull;
    mPlayfield = new GTitlePlayfield();
    AddProcess(mComputer2Process = new GComputerProcess(this, STYPE_PLAYER));
    AddProcess(new GTitleProcess());
  } else {
    mComputer2Process = ENull;
    mPlayfield = new GGamePlayfield();
    AddProcess(mPaddleProcess = new GPaddleProcess(this));
  }

  AddProcess(mComputer1Process = new GComputerProcess(this));
  AddProcess(mBallProcess = new GBallProcess(this));

  mPlayerScore = 0;
  mComputerScore = 0;
}

GGameState::~GGameState() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(COMMON_SLOT);
  delete mFont16;
  delete mFont8;
}

BSprite *GGameState::Ball() {
  return mBallProcess->mSprite;
}

BSprite *GGameState::Computer() {
  return mComputer1Process->mSprite;
}

BSprite *GGameState::Player() {
  if (mPaddleProcess) {
    return mPaddleProcess->mSprite;
  } else {
    return mComputer2Process->mSprite;
  }
}

void GGameState::PostRender() {
  if (mDemoMode) {
    return;
  }

  char scoreStr[20];
  TBCD score;
  BBitmap *bm = gDisplay.renderBitmap;

  // Player score
  score.FromUint32(mPlayerScore);
  score.ToString(scoreStr, ENull);
  bm->DrawStringShadow(ENull, scoreStr, mFont16, PLAYER_SCORE_X, PLAYER_SCORE_Y, COLOR_TEXT, COLOR_TEXT_SHADOW);

  // Computer score
  score.FromUint32(mComputerScore);
  score.ToString(scoreStr, ENull);
  bm->DrawStringShadow(ENull, scoreStr, mFont16, COMPUTER_SCORE_X, COMPUTER_SCORE_Y, COLOR_TEXT, COLOR_TEXT_SHADOW);

  if (mGameOver) {
    const char *gameOverStr;

    if (mPlayerScore > mComputerScore) {
      gameOverStr = "PLAYER WINS";
    } else {
      gameOverStr = "COMPUTER WINS";
    }

    bm->DrawStringShadow(ENull, gameOverStr, mFont16, (SCREEN_WIDTH - strlen(gameOverStr) * 16) / 2, GAME_OVER_Y, COLOR_TEXT, COLOR_TEXT_SHADOW);
    if (gControls.WasPressed(BUTTON_ANY)) {
      gGame->SetState(GAME_STATE_TITLE);
    }
  }
}

void GGameState::PlayerScores() {
  mPlayerScore++;
  Death();
}

void GGameState::ComputerScores() {
  mComputerScore++;
  Death();
}

void GGameState::Death() {
  if (mPlayerScore >= MAX_SCORE || mComputerScore >= MAX_SCORE) {
    if (!mDemoMode) {
      mGameOver = ETrue;
      return;
    }
  }

  if (mPaddleProcess) {
    mPaddleProcess->Reset();
  }

  if (mComputer2Process) {
    mComputer2Process->Reset();
  }

  mComputer1Process->Reset();
}
