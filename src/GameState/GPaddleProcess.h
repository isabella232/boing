#ifndef BOING_GPADDLEPROCESS_H
#define BOING_GPADDLEPROCESS_H

#include "Game.h"
class GGameState;

class GPaddleProcess : public BProcess {
public:
  GPaddleProcess(GGameState *aGameState);
  ~GPaddleProcess();
public:
  void Reset();
public:
  TBool RunBefore();
  TBool RunAfter();
  BSprite *mSprite;
protected:
  GGameState *mGameState;
};

#endif //BOING_GPADDLEPROCESS_H
