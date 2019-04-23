#ifndef BOING_GPADDLEPROCESS_H
#define BOING_GPADDLEPROCESS_H

#include "Game.h"
class GGameState;

class GPaddleProcess : public BProcess {
public:
  GPaddleProcess(GGameState *aGameState);
  ~GPaddleProcess();

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

#endif //BOING_GPADDLEPROCESS_H
