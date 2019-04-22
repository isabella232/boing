#include "GSplashProcess.h"

GSplashProcess::GSplashProcess() : BProcess() {}

GSplashProcess::~GSplashProcess() {}

TBool GSplashProcess::RunBefore() {
  return ETrue;
}

TBool GSplashProcess::RunAfter() {
  if (gControls.WasPressed(BUTTON_ANY)) {
    gGame->SetState(GAME_STATE_TITLE);
#ifdef ENABLE_AUDIO
    gSoundPlayer.SfxStartGame();
#endif
    return EFalse;
  }
  return ETrue;
}

