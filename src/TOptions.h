#ifndef BOING_TOPTIONS_H
#define BOING_TOPTIONS_H

#include "BTypes.h"
#include "BStore.h"
#include "Panic.h"

struct TOptions {
  TInt   version;
  TBool  muted;
  TFloat music;
  TFloat sfx;
  TFloat brightness;

  TOptions();

  void SetDefaults();

  void Reset(TBool aSave = ETrue);

  void Save();

  void Load();
};

#endif
