#ifndef BOING_GMUTEPROCESS_H
#define BOING_GMUTEPROCESS_H

#include "GGame.h"

class GMuteProcess : public BProcess {
  public:
    GMuteProcess() : BProcess() {
      mMuted = gOptions->muted;
      Reset();
    }

    ~GMuteProcess() {}

    void Reset() {
      mTimer = 15;
    }

    TBool RunBefore() {
      return ETrue;
    }

    TBool RunAfter() {
      mTimer--;
      if (gControls.CheckPressed(BUTTON2) && mTimer < 0) {
        Reset();
        mMuted = !mMuted;
        gOptions->muted = mMuted;
        gOptions->Save();
        gSoundPlayer.MuteMusic(mMuted);
      }
      return ETrue;
    }

    TInt  mTimer;
    TBool mMuted;
};

#endif //BOING_GMUTEPROCESS_H
