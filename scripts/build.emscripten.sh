#!/bin/sh

TARGET="emscripten"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="emcc"
CFLAGS="-Wall -Os -std=gnu11 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS=[png] -s WASM=1 --preload-file assets"
INCLUDE_PATHS="-I $DIR/bi-core/include -I $DIR/bi-ext/include "
LIBS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET -lbiext -lbi"

mkdir -p $DIR

# build bi-core
if [ ! -e $DIR/bi-core/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-core ]; then
    # git clone https://github.com/bismite/bi-core.git $DIR/bi-core
    cp -R ../bi-core $DIR/bi-core
  fi
  (cd $DIR/bi-core; make -f Makefile.$TARGET.mk clean all )
  if [ $? != 0 ]; then exit 1; fi
fi

# build bi-ext
if [ ! -e $DIR/bi-ext/build/$TARGET/libbiext.a ]; then
  if [ ! -e $DIR/bi-ext ]; then
    # git clone https://github.com/bismite/bi-ext.git $DIR/bi-ext
    cp -R ../bi-ext $DIR/bi-ext
  fi
  (cd $DIR/bi-ext; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../bi-core/include" )
  if [ $? != 0 ]; then exit 1; fi
fi

#
# build samples
#
for SRC in $SOURCES; do
# for SRC in src/timer.c src/hit.c; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -o $DIR/$NAME.html $SRC $CFLAGS $INCLUDE_PATHS $LIBS
  if [ $? != 0 ]; then exit 1; fi
done
