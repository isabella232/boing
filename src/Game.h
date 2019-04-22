#ifndef BOING_GAME_H
#define BOING_GAME_H

#define FRAME_RATE_INFO 0
#undef FRAME_RATE_INFO

#define ENABLE_AUDIO
//#undef ENABLE_AUDIO

#define ENABLE_OPTIONS
// #undef ENABLE_OPTIONS

#include <stdio.h>
#include <unistd.h>
#include <time.h>

#include <CreativeEngine.h>

#include "GResources.h"
#include "GGame.h"
#include "GSoundPlayer.h"
#include "GSplashState.h"
#include "GGameState.h"
#ifdef ENABLE_OPTIONS
#include "TOptions.h"
#endif

extern BViewPort   *gViewPort;
extern BGameEngine *gGameEngine;
#ifdef ENABLE_OPTIONS
extern TOptions    *gOptions;
#endif

#ifdef __XTENSA__
static const TInt MAX_BRIGHTNESS = 0x1fff;
static const TInt MIN_BRIGHTNESS = 0x50;
#endif

#endif //BOING_GAME_H
