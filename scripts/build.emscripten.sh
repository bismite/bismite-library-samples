#!/bin/sh

TARGET="emscripten"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="emcc"
CFLAGS="-Wall -Os -std=gnu11 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=[png] -s WASM=1 --preload-file assets -s ALLOW_MEMORY_GROWTH=1"
INCLUDE_PATHS="-I $DIR/bi-core/include -I $DIR/bi-ext/include "
LIBS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET -lbiext -lbi"

BI_CORE_DIR=$DIR/bi-core
BI_EXT_DIR=$DIR/bi-ext

mkdir -p $DIR

# build bi-core
if [ ! -d $BI_CORE_DIR ]; then
  if [ -z $BI_CORE ]; then
    git clone https://github.com/bismite/bi-core.git $BI_CORE_DIR
  else
    cp -R $BI_CORE $DIR
  fi
  (cd $BI_CORE_DIR; make -f Makefile.$TARGET.mk clean all )
  if [ $? != 0 ]; then exit 1; fi
fi
# build bi-ext
if [ ! -d $BI_EXT_DIR ]; then
  if [ -z $BI_EXT ]; then
    git clone https://github.com/bismite/bi-ext.git $BI_EXT_DIR
  else
    cp -R $BI_EXT $DIR
  fi
  (cd $BI_EXT_DIR; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../bi-core/include" )
  if [ $? != 0 ]; then exit 1; fi
fi

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -o $DIR/$NAME.html $SRC $CFLAGS $INCLUDE_PATHS $LIBS
  if [ $? != 0 ]; then exit 1; fi
done
