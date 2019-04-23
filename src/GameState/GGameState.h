#ifndef BOING_GGAMESTATE_H
#define BOING_GGAMESTATE_H

#include "Game.h"

#define MAX_SCORE TInt(9)

class GPaddleProcess;
class GComputerProcess;
class GBallProcess;

class GGameState : public BGameEngine {
public:
  GGameState(TBool aDemoMode=EFalse);
  ~GGameState();

  void PostRender();
  void Death();
  void PlayerScores();
  void ComputerScores();
  void Pause();
  void ResetBall();
  BSprite *Ball();
  BSprite *Computer();
  BSprite *Player();
  TBool Paused() {
    return mPaused;
  }
  TBool GameOver() {
    return mGameOver;
  }

private:
  TBool            mDemoMode;
  TBool            mGameOver;
  TBool            mPaused;
  BFont            *mFont8;
  BFont            *mFont16;
  GPaddleProcess   *mPaddleProcess;
  GComputerProcess *mComputer1Process;
  GComputerProcess *mComputer2Process;

public:
  TInt mPlayerScore;
  TInt mComputerScore;
  GBallProcess *mBallProcess;
};

#endif //BOING_GGAMESTATE_H
