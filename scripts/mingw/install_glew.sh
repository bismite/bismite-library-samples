#!/bin/bash

echo "* * * install GLEW for mingw * * *"

MINGW_DIR=build/mingw

GLEW_URL="https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0-win32.zip"

echo "Download ${GLEW_URL}"
mkdir -p build/download
curl --progress-bar -S -L -C - -o build/download/glew-2.1.0-win32.zip $GLEW_URL
unzip build/download/glew-2.1.0-win32.zip -d ${MINGW_DIR}

# copy
(cd ${MINGW_DIR}; mkdir -p lib include bin)
cp ${MINGW_DIR}/glew-2.1.0/lib/Release/x64/glew32.lib ${MINGW_DIR}/lib/
cp -R ${MINGW_DIR}/glew-2.1.0/include/* ${MINGW_DIR}/include/
cp ${MINGW_DIR}/glew-2.1.0/bin/Release/x64/*.dll ${MINGW_DIR}/bin/
