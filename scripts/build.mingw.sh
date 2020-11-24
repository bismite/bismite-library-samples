#!/bin/bash

TARGET="mingw"
DIR="${PWD}/build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="x86_64-w64-mingw32-gcc"
CFLAGS="-Wall -O3 -std=gnu11 "
INCLUDE_PATHS="-I ${DIR}/include -I ${DIR}/include/SDL2 -Dmain=SDL_main"
LDFLAGS="-lbismite-ext -lbismite-core -lglew32 -lopengl32 -lSDL2_image -lmingw32 -lSDL2main -lSDL2 -mwindows"
LIB_PATHS="-L $DIR/lib"

BI_CORE_DIR=$DIR/bismite-library-core
BI_EXT_DIR=$DIR/bismite-library-ext

mkdir -p $DIR/bin
mkdir -p $DIR/lib
mkdir -p $DIR/include

./scripts/$TARGET/install_glew.sh $DIR
./scripts/$TARGET/install_sdl.sh $DIR
./scripts/copy-bismite-libraries.sh $DIR

(cd $BI_CORE_DIR; make -f Makefile.$TARGET.mk all CFLAGS="${CFLAGS}" INCLUDE_PATHS="${INCLUDE_PATHS}" )
if [ $? != 0 ]; then exit 1; fi
cp $BI_CORE_DIR/build/$TARGET/*.a $DIR/lib/
cp -r $BI_CORE_DIR/include/bi $DIR/include/

(cd $BI_EXT_DIR; make -f Makefile.$TARGET.mk all CFLAGS="${CFLAGS}" INCLUDE_PATHS="${INCLUDE_PATHS}" )
if [ $? != 0 ]; then exit 1; fi
cp $BI_EXT_DIR/build/$TARGET/*.a $DIR/lib/
cp -r $BI_EXT_DIR/include/bi $DIR/include/

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  #echo "$CC -o $DIR/bin/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS"
  $CC -o $DIR/bin/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS
  if [ $? != 0 ]; then exit 1; fi
done
