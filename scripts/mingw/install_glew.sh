#!/bin/bash

MINGW_DIR=build/mingw
GLEW_URL="https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.tgz"
GLEW_MD5="3579164bccaef09e36c0af7f4fd5c7c7"
TGZFILE="build/download/glew-2.2.0.tgz"
GLEW_DIR="${MINGW_DIR}/glew-2.2.0"
CC=x86_64-w64-mingw32-gcc

mkdir -p build/download
mkdir -p ${MINGW_DIR}

MD5=($(md5sum ${TGZFILE}))

if [ "${MD5}" != ${GLEW_MD5} ]; then
  echo "Download ${GLEW_URL}"
  curl --progress-bar -S -L -o ${TGZFILE} ${GLEW_URL}
fi
tar zxf ${TGZFILE} -C ${MINGW_DIR}

# build
(
 cd ${GLEW_DIR} ;
 x86_64-w64-mingw32-gcc -DGLEW_NO_GLU -O2 -Wall -W -Iinclude  -DGLEW_BUILD -o src/glew.o -c src/glew.c ;
 $CC -shared -Wl,-soname,libglew32.dll -Wl,--out-implib,lib/libglew32.dll.a -o lib/glew32.dll src/glew.o -L/mingw/lib -lopengl32 -nostdlib
)

# copy
(cd ${MINGW_DIR}; mkdir -p lib include bin)
cp ${GLEW_DIR}/lib/* ${MINGW_DIR}/lib/
cp -R ${GLEW_DIR}/include/* ${MINGW_DIR}/include/
cp ${GLEW_DIR}/lib/*.dll ${MINGW_DIR}/bin/
