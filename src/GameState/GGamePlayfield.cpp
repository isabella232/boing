#include "GGamePlayfield.h"

GGamePlayfield::GGamePlayfield() {
#ifdef ENABLE_AUDIO
  gSoundPlayer.PlayMusic(STAGE_4_XM);
#endif
  gResourceManager.LoadBitmap(GAME_BMP, BKG_SLOT, IMAGE_ENTIRE);
  mBackground = gResourceManager.GetBitmap(BKG_SLOT);
  gDisplay.SetPalette(mBackground);

}

GGamePlayfield::~GGamePlayfield() {
  gResourceManager.ReleaseBitmapSlot(BKG_SLOT);
}

void GGamePlayfield::Render() {
  gDisplay.renderBitmap->CopyPixels(mBackground);
}

