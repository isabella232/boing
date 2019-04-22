#ifndef BOING_GSPLASHPLAYFIELD_H
#define BOING_GSPLASHPLAYFIELD_H

#include "Game.h"

class GSplashPlayfield : public BPlayfield {
public:
  GSplashPlayfield();

  virtual ~GSplashPlayfield();

  void Render();

public:
  BBitmap *mBackground;
};

#endif //BOING_GSPLASHPLAYFIELD_H
