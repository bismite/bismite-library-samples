#!/bin/bash

SDL2_URL="https://www.libsdl.org/release/SDL2-devel-2.0.10-mingw.tar.gz"
SDL2_MD5="9557cc8b9a6380b6c18a4ec29c575274"
SDL2_IMAGE_URL="https://www.libsdl.org/projects/SDL_image/release/SDL2_image-devel-2.0.5-mingw.tar.gz"
SDL2_IMAGE_MD5="2704080296a24b765ce1e9c3c427a970"
SDL2_MIXER_URL="https://www.libsdl.org/projects/SDL_mixer/release/SDL2_mixer-devel-2.0.4-mingw.tar.gz"
SDL2_MIXER_MD5="067275e50b40aed65dc472b9964c1d38"

DIR=build/mingw
mkdir -p $DIR
mkdir -p build/download

function dl () {
  FILENAME=$(basename ${1})
  MD5=($(md5sum build/download/${FILENAME}))
  if [ "${MD5}" != ${2} ]; then
    echo "Download ${1}"
    curl --progress-bar -S -L ${1} > build/download/${FILENAME}
  fi
  tar xzf build/download/${FILENAME} -C ${DIR}

  MINGW_INSTALL_DIR=${PWD}/${DIR}/
  mkdir -p ${MINGW_INSTALL_DIR}/x86_64-w64-mingw32
  (cd ${3}; make cross CROSS_PATH=${MINGW_INSTALL_DIR}/ ARCHITECTURES=x86_64-w64-mingw32 > /dev/null)
}

dl $SDL2_URL $SDL2_MD5 "${DIR}/SDL2-2.0.10"
dl $SDL2_IMAGE_URL $SDL2_IMAGE_MD5 "${DIR}/SDL2_image-2.0.5"
dl $SDL2_MIXER_URL $SDL2_MIXER_MD5 "${DIR}/SDL2_mixer-2.0.4"

mkdir -p ${DIR}/include ${DIR}/lib ${DIR}/bin
cp -r ${DIR}/x86_64-w64-mingw32/include/* ${DIR}/include
cp -r ${DIR}/x86_64-w64-mingw32/lib/* ${DIR}/lib
cp -r ${DIR}/x86_64-w64-mingw32/bin/*.dll ${DIR}/bin
