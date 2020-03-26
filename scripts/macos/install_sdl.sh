#!/bin/sh

echo "Install SDL"

DL_DIR=$1

mkdir -p $DL_DIR

FRAMEWORKS_DIR=$HOME/Library/Frameworks
mkdir -p $FRAMEWORKS_DIR

# SDL2
SDL2_FRAMEWORK=SDL2.framework
SDL2_DMG=SDL2-2.0.10.dmg
if [ ! -e $FRAMEWORKS_DIR/$SDL2_FRAMEWORK ]; then
  if [ ! -e $DL_DIR/$SDL2_DMG ]; then
    curl --progress-bar -S -L -o $DL_DIR/$SDL2_DMG https://www.libsdl.org/release/$SDL2_DMG
  fi
  hdiutil attach $DL_DIR/$SDL2_DMG
  cp -R /Volumes/SDL2/$SDL2_FRAMEWORK $FRAMEWORKS_DIR
  hdiutil detach /Volumes/SDL2
fi

# SDL2_image
SDL2_IMAGE_FRAMEWORK=SDL2_image.framework
SDL2_IMAGE_DMG=SDL2_image-2.0.5.dmg
if [ ! -e $FRAMEWORKS_DIR/$SDL2_IMAGE_FRAMEWORK ]; then
  if [ ! -e $DL_DIR/$SDL2_IMAGE_DMG ]; then
    curl --progress-bar -S -L -o $DL_DIR/$SDL2_IMAGE_DMG https://www.libsdl.org/projects/SDL_image/release/$SDL2_IMAGE_DMG
  fi
  hdiutil attach $DL_DIR/$SDL2_IMAGE_DMG
  cp -R /Volumes/SDL2_image/$SDL2_IMAGE_FRAMEWORK $FRAMEWORKS_DIR
  hdiutil detach /Volumes/SDL2_image
fi

#SDL2_mixer
SDL2_MIXER_FRAMEWORK=SDL2_mixer.framework
SDL2_MIXER_DMG=SDL2_mixer-2.0.4.dmg
if [ ! -e $FRAMEWORKS_DIR/$SDL2_MIXER_FRAMEWORK ]; then
  if [ ! -e $DL_DIR/$SDL2_MIXER_DMG ]; then
    curl --progress-bar -S -L -o $DL_DIR/$SDL2_MIXER_DMG https://www.libsdl.org/projects/SDL_mixer/release/$SDL2_MIXER_DMG
  fi
  hdiutil attach $DL_DIR/$SDL2_MIXER_DMG
  cp -R /Volumes/SDL2_mixer/$SDL2_MIXER_FRAMEWORK $FRAMEWORKS_DIR
  hdiutil detach /Volumes/SDL2_mixer
fi

# header
mkdir -p build/macos/include/SDL2
cp -R ${FRAMEWORKS_DIR}/${SDL2_FRAMEWORK}/Headers/* build/macos/include/SDL2/
cp -R ${FRAMEWORKS_DIR}/${SDL2_IMAGE_FRAMEWORK}/Headers/* build/macos/include/SDL2/
cp -R ${FRAMEWORKS_DIR}/${SDL2_MIXER_FRAMEWORK}/Headers/* build/macos/include/SDL2/
