// Created by Jesus Garcia on 10/2/18.
//

#include "Game.h"
#include "GSoundPlayer.h"
#include "GResources.h"
#include "Memory.h"

//#define DISABLE_AUDIO
//#undef DISABLE_AUDIO

#define MUSIC_VOLUME (.20)
#define SFX_VOLUME (.35)

GSoundPlayer gSoundPlayer;

static const TUint16 effectsList[] = {
  // SFX_EMPTY_WAV,
  SFX_BOUNCE_OFF_PLAYER_WAV,
  SFX_BOUNCE_WALL_WAV,
  SFX_KILL_BLOCK_WAV,
  SFX_START_GAME_WAV,
  SFX_NEW_BALL_WAV,
};

static TUint16 FindSfxNumber(TUint16 aSfxFile) {

  for (size_t i = 0; i < sizeof(effectsList); i++) {
    if (aSfxFile == effectsList[i]) {
      return (TUint16) i; // Should not go above UINT16_MAX
    }
  }
  printf("WARNING :: Could not find SFX for ID %i\n", aSfxFile);

  return UINT16_MAX;
}

static const TUint16 allSongs[] = {
  EMPTYSONG_XM,
  INTRO_XM,
  STAGE_1_XM,
  STAGE_2_XM,
  STAGE_3_XM,
  STAGE_4_XM,
  STAGE_5_XM,
  GAMEOVER_XM,
};

GSoundPlayer::~GSoundPlayer() {
  // Release songs memory
  FreeMem(mSongSlots);
}

void GSoundPlayer::Init(TUint8 aNumberFxChannels) {
#ifdef DISABLE_AUDIO
  return;
#endif
  mMaxSongs = sizeof(allSongs) / sizeof(TUint16);
  mMaxEffects = sizeof(effectsList) / sizeof(TUint16);

  soundEngine.InitAudioEngine(aNumberFxChannels, mMaxEffects);

//  SDL_ClearError();
  LoadEffects();
//  SDL_ClearError();



  mSongSlots = (SongSlot *)AllocMem(sizeof(SongSlot) * mMaxSongs, MEMF_SLOW);

//  for (TUint8 i = 0; i < mMaxSongs; i++) {
//    auto *slot = (SongSlot *)AllocMem(sizeof(SongSlot), MEMF_SLOW);
//
//    slot->mResourceNumber = allSongs[i];
//    slot->mSlotNumber = SONG0_SLOT + i;
//
//    gResourceManager.LoadRaw(allSongs[i], slot->mSlotNumber);
//    slot->mRaw = gResourceManager.GetRaw(slot->mSlotNumber);
//
//    mSongSlots[i] = *slot;
//    FreeMem(slot);
//  }
  PlayMusic(EMPTYSONG_XM);


  soundEngine.SetMusicVolume(gOptions->music);
  soundEngine.SetEffectsVolume(gOptions->sfx);
  soundEngine.MuteMusic(gOptions->muted);
}

TBool GSoundPlayer::PlayMusic(TInt16 aResourceId) {
#ifdef DISABLE_AUDIO
  return ETrue;
#endif
  //  aResourceId = EMPTYSONG_XM;
//  printf("%s %i\n", __PRETTY_FUNCTION__, aResourceId);

  BRaw *songToLoad = FindRawSongFileById(aResourceId);

  TBool music = soundEngine.PlayMusic(songToLoad, aResourceId);
//   BSoundEngine::PlayMusic un-mutes the music
// We have to re-mute it in case of mute == true

  soundEngine.SetMusicVolume(gOptions->music);
  soundEngine.SetEffectsVolume(gOptions->sfx);
  soundEngine.MuteMusic(gOptions->muted);

  return music;
}


BRaw *GSoundPlayer::FindRawSongFileById(TInt16 aResourceId) {

  for (TUint8 i = 0; i < mMaxSongs; i++) {
    if (allSongs[i] == aResourceId) {
      if (gResourceManager.GetRaw(SONG0_SLOT)) {
        gResourceManager.ReleaseRawSlot(SONG0_SLOT);
      }
      gResourceManager.LoadRaw(allSongs[i], SONG0_SLOT);
      return gResourceManager.GetRaw(SONG0_SLOT);;
    }
  }

  printf("WARNING :: Could not find song %i\n", aResourceId);

  return ENull;
}


TBool GSoundPlayer::LoadEffects() {
#ifdef DISABLE_AUDIO
  return ETrue;
#endif
  for (TUint8 index = 0; index < mMaxEffects; index++) {
    soundEngine.LoadEffect(index, effectsList[index], SFX1_SLOT + index);
//    return ETrue;
  }
  return ETrue;
}


void  GSoundPlayer::MuteMusic(TBool aMuted) {
  return soundEngine.MuteMusic(aMuted);
}



void GSoundPlayer::TriggerSfx(TUint16 aSfxNumber, TInt8 aChannel) {
#ifdef DISABLE_AUDIO
  return;
#endif
  soundEngine.PlaySfx(FindSfxNumber(aSfxNumber), aChannel);
}

void GSoundPlayer::SfxBounceOffPlayer() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_BOUNCE_OFF_PLAYER_WAV), 0);
}


void GSoundPlayer::SfxBounceWall() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_BOUNCE_WALL_WAV), 0);
}

void GSoundPlayer::SfxStartGame() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_START_GAME_WAV), 0);
}

void GSoundPlayer::SfxNewBall() {
  soundEngine.PlaySfx(FindSfxNumber(SFX_NEW_BALL_WAV), 0);
}
