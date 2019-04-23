#ifndef BOING_GPAUSEPROCESS_H
#define BOING_GPAUSEPROCESS_H

#include "GGame.h"

class GPauseProcess : public BProcess {
  public:
    GPauseProcess(GGameState *aGameState) : BProcess() {
      mGameState = aGameState;
      Reset();
    }

    ~GPauseProcess() {}

    void Reset() {
      mTimer = 15;
    }

    TBool RunBefore() {
      return ETrue;
    }

    TBool RunAfter() {
      mTimer--;
      if (gControls.CheckPressed(BUTTON_START) && mTimer < 0) {
        gSoundPlayer.SfxStartGame();

        if (!gOptions->muted) {
          if (mGameState->Paused()) {
            gSoundPlayer.SetMusicVolume(gOptions->music);
          } else {
            gSoundPlayer.SetMusicVolume(0);
          }
        }

        Reset();
        mGameState->Pause();
      }
      return ETrue;
    }

    TInt       mTimer;
    GGameState *mGameState;
};

#endif //BOING_GPAUSEPROCESS_H
