#ifndef BOING_GCOMPUTERPROCESS_H
#define BOING_GCOMPUTERPROCESS_H

#include "Game.h"
class GGameState;

class GComputerProcess : public BProcess {
public:
  GComputerProcess(GGameState *aGameState, TUint32 aType=STYPE_ENEMY);
  ~GComputerProcess();
public:
  void Reset();
public:
  TBool RunBefore();
  TBool RunAfter();
  BSprite *mSprite;
protected:
  GGameState *mGameState;
};

#endif //BOING_GCOMPUTERPROCESS_H
