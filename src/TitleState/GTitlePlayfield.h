#ifndef BOING_GTITLEPLAYFIELD_H
#define BOING_GTITLEPLAYFIELD_H

#include "Game.h"

class GTitlePlayfield : public BPlayfield {
public:
  GTitlePlayfield();
  virtual ~GTitlePlayfield();
  void Render();
public:
  BBitmap *mBackground;
};


#endif //BOING_GTITLEPLAYFIELD_H
