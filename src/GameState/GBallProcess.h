#ifndef BOING_GBALLPROCESS_H
#define BOING_GBALLPROCESS_H

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

  TBool RunBefore();
  TBool RunAfter();
  BSprite *Sprite() {
    return mSprite;
  }
  void Pause(TBool aPause);
  void Reset(TFloat aVelocity);
  void Reset() {
    Reset(VELOCITY);
  }

private:
  TBool WaitState();
  TBool MoveState();
  void SpeedUp();
  void CheckCollision(BSprite *paddleSprite);

private:
  TInt       mAngle;
  TFloat     mVelocity;
  BSprite    *mSprite;
  GGameState *mGameState;
  enum {
    WAIT_STATE,
    MOVE_STATE,
  } mState;
};

#endif //BOING_GBALLPROCESS_H
