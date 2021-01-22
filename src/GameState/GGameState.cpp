#include "Game.h"

#include "GGamePlayfield.h"
#include "GTitlePlayfield.h"
#include "GPaddleProcess.h"
#include "GComputerProcess.h"
#include "GBallProcess.h"
#include "GTitleProcess.h"
#include "GPauseProcess.h"
#include "GGetReadyProcess.h"

const TInt PLAYER_SCORE_X = 4;
const TInt PLAYER_SCORE_Y = 4;
const TInt COMPUTER_SCORE_X = SCREEN_WIDTH - 20;
const TInt COMPUTER_SCORE_Y = 4;
const TInt SCREEN_CENTER = SCREEN_HEIGHT / 2 - 8;

GGameState::GGameState(TBool aDemoMode) : BGameEngine(gViewPort) {
  mDemoMode = aDemoMode;
  mGameOver = EFalse;
  mPaused = EFalse;

  mFont8  = new BFont(gResourceManager.GetBitmap(FONT_8x8_SLOT), FONT_16x16);
  mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);

  gResourceManager.LoadBitmap(GAME_SPRITES_BMP, COMMON_SLOT, IMAGE_16x16);
  gResourceManager.LoadBitmap(PLAYER_SPRITES_BMP, PLAYER_SLOT, IMAGE_8x32);

  AddProcess(mComputer1Process = new GComputerProcess(this));
  AddProcess(mBallProcess = new GBallProcess(this));

  if (aDemoMode) {
    mPaddleProcess = ENull;
    mPlayfield = new GTitlePlayfield();
    mBallProcess->Reset();
    AddProcess(mComputer2Process = new GComputerProcess(this, STYPE_PLAYER));
    AddProcess(new GTitleProcess());
  } else {
    // Reset any previously held keys
    gControls.cKeys = 0;

    mComputer2Process = ENull;
    mPlayfield = new GGamePlayfield();
    AddProcess(mPaddleProcess = new GPaddleProcess(this));
    AddProcess(new GPauseProcess(this));
    AddProcess(new GGetReadyProcess(this));
  }

  mPlayerScore = 0;
  mComputerScore = 0;
}

GGameState::~GGameState() {
  gResourceManager.ReleaseBitmapSlot(PLAYER_SLOT);
  gResourceManager.ReleaseBitmapSlot(COMMON_SLOT);
  delete mFont16;
  delete mFont8;
}

void GGameState::Pause() {
  mPaused = !mPaused;
  mPaddleProcess->Pause(mPaused);
  mComputer1Process->Pause(mPaused);
  mBallProcess->Pause(mPaused);
}

BSprite *GGameState::Ball() {
  return mBallProcess->Sprite();
}

BSprite *GGameState::Computer() {
  return mComputer1Process->Sprite();
}

BSprite *GGameState::Player() {
  if (mPaddleProcess) {
    return mPaddleProcess->Sprite();
  }
  return mComputer2Process->Sprite();
}

void GGameState::ResetBall() {
  mBallProcess->Reset();
}

void GGameState::PostRender() {
  if (mDemoMode) {
    return;
  }

  BBitmap *bm = gDisplay.renderBitmap;

  if (mPaused) {
    bm->DrawStringShadow(ENull, "PAUSE", mFont16, (SCREEN_WIDTH - 5 * 16) / 2, SCREEN_CENTER, COLOR_TEXT, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT);
  } else {
    char scoreStr[20];
    TBCD score;

    // Player score
    score.FromUint32(mPlayerScore);
    score.ToString(scoreStr, ENull);
    bm->DrawStringShadow(ENull, scoreStr, mFont16, PLAYER_SCORE_X, PLAYER_SCORE_Y, COLOR_TEXT, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT);

    // Computer score
    score.FromUint32(mComputerScore);
    score.ToString(scoreStr, ENull);
    bm->DrawStringShadow(ENull, scoreStr, mFont16, COMPUTER_SCORE_X, COMPUTER_SCORE_Y, COLOR_TEXT, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT);
  }

  if (mGameOver) {
    const char *gameOverStr;

    if (mPlayerScore > mComputerScore) {
      gameOverStr = "PLAYER WINS";
    } else {
      gameOverStr = "COMPUTER WINS";
    }

    bm->DrawStringShadow(ENull, gameOverStr, mFont16, (SCREEN_WIDTH - strlen(gameOverStr) * 16) / 2, SCREEN_CENTER, COLOR_TEXT, COLOR_TEXT_SHADOW, COLOR_TEXT_TRANSPARENT);
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

  if (mDemoMode) {
    mBallProcess->Reset();
  } else {
    AddProcess(new GGetReadyProcess(this));
  }
}
