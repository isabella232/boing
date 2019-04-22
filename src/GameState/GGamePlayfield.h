#ifndef BOING_GGAMEPLAYFIELD_H
#define BOING_GGAMEPLAYFIELD_H

#include "Game.h"

class GGamePlayfield : public BPlayfield {
public:
  GGamePlayfield();

  virtual ~GGamePlayfield();

  void Render();

public:
  BBitmap *mBackground;
};


#endif //BOING_GGAMEPLAYFIELD_H
