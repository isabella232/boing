#ifndef GENUS_GSPLASHPROCESS_H
#define GENUS_GSPLASHPROCESS_H

#include "Game.h"

class GSplashProcess : public BProcess {
public:
  GSplashProcess();
  ~GSplashProcess();

public:
  TBool RunBefore();
  TBool RunAfter();
};

#endif //GENUS_GSPLASHPROCESS_H
