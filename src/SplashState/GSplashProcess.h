#ifndef BOING_GSPLASHPROCESS_H
#define BOING_GSPLASHPROCESS_H

#include "Game.h"

class GSplashProcess : public BProcess {
public:
  GSplashProcess();
  ~GSplashProcess();

public:
  TBool RunBefore();
  TBool RunAfter();
};

#endif //BOING_GSPLASHPROCESS_H
