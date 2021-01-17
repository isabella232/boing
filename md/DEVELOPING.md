# Developer guide
This document's purpose is to provide a high-level understanding of how Boing works. Programmers wishing to get involved should review our [contribution](./md/CONTRIBUTING.md) guidelines as well as have a decent understanding of C++ and build tools.
Boing is a cross-platform game developed by [Modus Create](https://moduscreate.com) and its sole purpose is to demonstrate how to use [creative engine](https://github.com/moduscreateorg/creative-engine). The Boing's target platform is the [LDK Game](https://retromimi.com/products/ldk-game) console, and it can be compiled and run on macOS and Linux (Ubuntu and Arch distributions).

## How to Play
Boing is a simple game where you have to outsmart your AI opponent and land the ball behind his paddle.

![boing-gameplay.gif](./img/boing-gameplay.gif)

The following is a list of dependencies for each platform.

## Quickstart
It is possible to setup your local environment with the provided scripts:
- `scripts/build.sh` - Sets up all desktop dependencies and builds
- `scripts/build-ldk.sh` - Builds and flashes the game to LDK (does not install toolchain)

### Dependencies (All platforms)
[Boing](https://github.com/moduscreateorg/boing), this game.\
[Creative Engine](https://github.com/ModusCreateOrg/creative-engine) is the game engine developed by Modus Create. It implements LibXMP, SDL2, ESP-IDF (Audio, Video and Input drivers).\
[LibXMP](http://xmp.sourceforge.net/) is a fantastic cross-platform library for playing music using the [Xtended Module (XM)](https://en.wikipedia.org/wiki/XM_(file_format)) format and also has additional functionality to play sound effects.\
[Rcomp](https://github.com/ModusCreateOrg/creative-engine/blob/master/tools/rcomp.cpp) is a CLI tool that takes any binary resources and packages  (graphic, audio, etc.) them into a binary blob to be included in the game executable and is part of [Creative Engine](https://github.com/ModusCreateOrg/creative-engine).\
[SDL2](https://www.libsdl.org/download-2) is a cross-platform low-level media layer framework. Creative Engine generates audio data with LibXMP and feeds it into the SDL2 audio run loop and does similar to present visuals in the application window as well as poll for keyboard input to allow for game play.\
[Buildroot](https://github.com/buildroot/buildroot) BuildRoot is an easy to use solution designed to create Linux distributions and cross-compiler toolchains for embedded development. The RetroFW project uses buildroot as a common solution so that all developers can quickly and easily begin software development.

## Getting started
Let's get setup for desktop and device development. To do so, we'll need to ensure that we have the right libraries and tools installed.

We're going to get setup in two phases:
1. Install and setup dependencies
2. Setup toolchain for LDK builds and flashing

## Clone Boing and Creative Engine
The first thing we need to is create a folder that will contain Boing and Creative engine. When we're done, the folder struction will look similar to the following.

    projects/
       |-boing-game/
            |-creative-engine/      # Source Creative Engine
            |-boing/                # Source for Boing
                |-creative-engine/  # Symbolic Link to ../creative-engine/

Let's clone the Boing and Creative Engine repos:

    # Would be within ~/projects or similar
    mkdir boing-game/
    cd boing-game/
    git clone git@github.com:ModusCreateOrg/boing.git
    git clone git@github.com:ModusCreateOrg/creative-engine.git -b develop
    ln -s creative-engine/ boing/creative-engine               # Create Symbolic Link

## Install dependencies

### macOS
- [ ] Install [XCode](https://developer.apple.com/xcode/) CLI tools

```
xcode-select --install
```

- [ ] Install [Homebrew](https://brew.sh)

```
/usr/bin/ruby -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"`
```
- [ ] Install final dependencies via HomeBrew
```
# Run this command from boing/
brew install
```
- [ ] Build and run Boing
```
# Run these commands command from boing/
make rcomp resources    # Build necessary tooling & resources binary
mkdir build             # Create build directory
cd build
cmake ..                # Execute Cmake and prepare for build
make -j 4               # Four threads. Change to whatever you wish for faster compilation
open Boing.app          # Run Boing
```

### Linux (Debian based)
- [ ] Install dependencies
```
sudo apt-get install build-essential libsdl2-dev libsdl2-image-dev cmake g++ -y

```
- [ ] Build Boing
```
# Run this command from boing/
make rcomp resources       # Generates src/Resources.bin binary file to be included in our program
mkdir build          # Create build directory
cd build
cmake ..             # Execute Cmake and prepare for build
make -j 4            # Four threads. Change to whatever you wish for faster compilation
./Boing
```

## Buildroot setup for LDK
We will follow [this guide](https://docs.google.com/document/d/19kJXO3EZ8XCoeporuUUgV_S93AaPbSagza3sAgBILu8/edit#heading=h.nhg2jcflf9wl) with some minor changes.

Install compiler dependencies:

`sudo apt-get install build-essential libncurses5 libncurses5-dev git python unzip bc squashfs-tools`

Download this specific [buildroot snapshot](https://buildroot.org/downloads/buildroot-2018.02.9.tar.gz) as it was confirmed to be working.

```
cd /opt
wget https://buildroot.org/downloads/buildroot-2018.02.9.tar.gz
tart -xzvf buildroot-2018.02.9.tar.gz
cd buildroot-2018.02.9
make menuconfig
```

Set the following options in the buildroot interface:

##### Target Options

- Target Architecture - MIPS (little endian)
- Disable soft-float

##### Toolchain

- C library (uClibc-ng)
- Enable WCHAR support
- Enable C++ support

##### Target Packages

- Graphic libraries and applications (graphic/text)
- SDL
- SDL_gfx
- SDL_image
- SDL_mixer
- SDL_net
- SDL_sound
- SDL_TTF

Exit the menu system and save the configuration when prompted. We are now ready to run make on the project. This process might take a while.

```
export FORCE_UNSAFE_CONFIGURE=1
make -j 4            # Four threads. Change to whatever you wish for faster compilation
```

Install the toolchain to a common collcation

```
mkdir ~/ldk/mipsel-linux-uclibc
cp -r /opt/buildroot-2018.02.9/output/host/* ~/ldk/mipsel-linux-uclibc
```

We now have a toolchain that we can use to develop RS-97 software. The final step is to add the tools to our path.
You can add the following to `/etc/profile.d/rs97tools.sh` or straight into your `.bashrc` or `.XYZrc` files. All that matters is that the tools directories are in your $PATH.

```
export PATH=$HOME/ldk/mipsel-linux-uclibc/mipsel-buildroot-linux-uclibc/sysroot/usr/bin:$PATH
export PATH=$HOME/ldk/mipsel-linux-uclibc/bin:$PATH
```

Log out and back in to confirm the setup was done correctly:

```
mipsel-linux-gcc
mipsel-linux-gcc.br_real: fatal error: no input files
compilation terminated.
```

```
sdl-config --libs
-L/opt/buildroot-2018.02.9/output/host/mipsel-buildroot-linux-uclibc/sysroot/usr/lib -lSDL -lpthread
```

## Build and Flash LDK

Run `scripts/make-ldk.sh` to compile the game for the LDK.
To flash the game to LDK, connect it first and run `scripts/build-ldk.sh` which will compile the code and flash it to the device.


## Additional information
We highly recommend using the [CLion IDE](https://www.jetbrains.com/clion/) to develop the project and run the builds.


## Git Workflow
Read about [GitHub workflow](https://github.com/ModusCreateOrg/creative-engine) at the creative-engine repo.

The gist is that we fork the main repos and work in our private forks.  We push to our forks.  We create Pull Requests against the main repos.

The only way code makes it into master in the main repo is if we merge a PR.
