#!/bin/sh

TARGET="emscripten"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="emcc"
CFLAGS="-Wall -Os -std=gnu11 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=[png] -s WASM=1 --preload-file assets -s ALLOW_MEMORY_GROWTH=1"
INCLUDE_PATHS="-I $DIR/include"
LIBS="-L $DIR/lib -lbismite-ext -lbismite-core"

BI_CORE_DIR=$DIR/bismite-library-core
BI_EXT_DIR=$DIR/bismite-library-ext

mkdir -p $DIR/bin
mkdir -p $DIR/lib
mkdir -p $DIR/include

sh ./scripts/copy-bismite-libraries.sh $DIR

(cd $BI_CORE_DIR; make -f Makefile.$TARGET.mk all )
if [ $? != 0 ]; then exit 1; fi
cp $BI_CORE_DIR/build/$TARGET/*.a $DIR/lib/
cp -r $BI_CORE_DIR/include/bi $DIR/include/

(cd $BI_EXT_DIR; make -f Makefile.$TARGET.mk all INCLUDE_PATHS="-I ../include" )
if [ $? != 0 ]; then exit 1; fi
cp $BI_EXT_DIR/build/$TARGET/*.a $DIR/lib/
cp -r $BI_EXT_DIR/include/bi $DIR/include/

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -o $DIR/bin/${NAME}.html $SRC $CFLAGS $INCLUDE_PATHS $LIBS
  if [ $? != 0 ]; then exit 1; fi
done
