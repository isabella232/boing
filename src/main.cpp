#include "Game.h"

BViewPort *gViewPort;
BGameEngine *gGameEngine;
GGame *gGame;

#ifdef ENABLE_OPTIONS
TOptions *gOptions;
#endif

// app_main
extern "C" void app_main() {
  gGame = new GGame();
  gGame->Run();
  delete gGame;
}

#ifndef __XTENSA
int main() {
  app_main();
  return 0;
}
#endif
