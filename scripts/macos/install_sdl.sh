#!/bin/bash

mkdir -p build/download

URL=https://github.com/bismite/SDL-macOS-UniversalBinaries/releases/download/main/SDL-macOS-UniversalBinaries.tgz
SDL_TGZ=SDL-macOS-UniversalBinaries.tgz

if [ ! -e build/download/${SDL_TGZ} ]; then
  curl --progress-bar -S -L -o build/download/${SDL_TGZ} ${URL}
fi
tar zxf build/download/${SDL_TGZ} -C build/macos

cp -R build/macos/SDL-macOS-UniversalBinaries/* build/macos/
