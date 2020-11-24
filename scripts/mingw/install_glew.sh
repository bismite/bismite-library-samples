#!/bin/bash

MINGW_DIR=build/mingw

GLEW_URL="https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0-win32.zip"
GLEW_MD5="32a72e6b43367db8dbea6010cd095355"
ZIPFILE="build/download/glew-2.1.0-win32.zip"

mkdir -p build/download
MD5=($(md5sum ${ZIPFILE}))

if [ "${MD5}" != ${GLEW_MD5} ]; then
  echo "Download ${GLEW_URL}"
  curl --progress-bar -S -L -o ${ZIPFILE} ${GLEW_URL}
fi
unzip -oq ${ZIPFILE} -d ${MINGW_DIR}

# copy
(cd ${MINGW_DIR}; mkdir -p lib include bin)
cp ${MINGW_DIR}/glew-2.1.0/lib/Release/x64/glew32.lib ${MINGW_DIR}/lib/
cp -R ${MINGW_DIR}/glew-2.1.0/include/* ${MINGW_DIR}/include/
cp ${MINGW_DIR}/glew-2.1.0/bin/Release/x64/*.dll ${MINGW_DIR}/bin/
