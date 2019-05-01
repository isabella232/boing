#include "Game.h"

class GGetReadyProcess : public BProcess {
  public:
    GGetReadyProcess(GGameState *aGameState) : BProcess() {
      mTimer = 30;
      mGameState = aGameState;
      mFont16 = new BFont(gResourceManager.GetBitmap(FONT_16x16_SLOT), FONT_16x16);
    }

    ~GGetReadyProcess() {
      delete mFont16;
    }

    TBool RunBefore() {
      return ETrue;
    }

    TBool RunAfter() {
      // Don't allow pausing the game in this state
      gControls.cKeys &= ~BUTTON_START;

      if (mTimer-- < 0 || gControls.IsPressed(BUTTON_ANY)) {
        mGameState->ResetBall();
        return EFalse;
      }

      gDisplay.renderBitmap->DrawStringShadow(
        ENull,
        "GET READY",
        mFont16,
        (SCREEN_WIDTH - 9 * 16) / 2,
        SCREEN_HEIGHT / 2 - 8,
        COLOR_TEXT,
        COLOR_TEXT_SHADOW
      );

      return ETrue;
    }

    TInt       mTimer;
    BFont      *mFont16;
    GGameState *mGameState;
};
