#ifndef GENUS_GBALLPROCESS_H
#define GENUS_GBALLPROCESS_H

#include "Game.h"

#define VELOCITY TInt(6)
#define MAX_VELOCITY TInt(24)
#define ANGLE_RANGE TInt(24)
#define LEFT_ANGLE TInt(180 - ANGLE_RANGE / 2)
#define RIGHT_ANGLE TInt(0 - ANGLE_RANGE / 2)

class GBallProcess : public BProcess {
public:
  GBallProcess(GGameState *aGameState, TFloat aVelocity = VELOCITY);
  virtual ~GBallProcess();
public:
  TBool RunBefore();
  TBool RunAfter();
public:
  void SpeedUp();
  // reposition the ball at start of round
  void Reset(TFloat aVelocity);
  void Reset() {
    Reset(mVelocity);
  }
  void CheckCollision(BSprite *paddleSprite);
public:
  BSprite *mSprite;
private:
  TInt   mAngle;
  TFloat mVelocity;
  GGameState *mGameState;
};

#endif //GENUS_GBALLPROCESS_H
