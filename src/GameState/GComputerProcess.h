#ifndef BOING_GCOMPUTERPROCESS_H
#define BOING_GCOMPUTERPROCESS_H

#include "Game.h"
class GGameState;

class GComputerProcess : public BProcess {
public:
  GComputerProcess(GGameState *aGameState, TUint32 aType=STYPE_ENEMY);
  ~GComputerProcess();

  TBool RunBefore();
  TBool RunAfter();
  BSprite *Sprite() {
    return mSprite;
  }
  void Pause(TBool aPause);
  void Reset();

private:
  TBool WaitState();
  TBool MoveState();

private:
  BSprite    *mSprite;
  GGameState *mGameState;
  enum {
    WAIT_STATE,
    MOVE_STATE,
  } mState;
};

#endif //BOING_GCOMPUTERPROCESS_H
