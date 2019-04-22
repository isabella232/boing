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
public:
  void Death();
  void PlayerScores();
  void ComputerScores();
  BSprite *Ball();
  BSprite *Computer();
  BSprite *Player();
  TBool GameOver() {
    return mGameOver;
  }
private:
  TBool            mDemoMode;
  TBool            mGameOver;
  BFont *mFont8,   *mFont16;
  GPaddleProcess   *mPaddleProcess;
  GComputerProcess *mComputer1Process;
  GComputerProcess *mComputer2Process;
public:
  TInt mPlayerScore;
  TInt mComputerScore;
  GBallProcess *mBallProcess;
};

#endif //BOING_GGAMESTATE_H
