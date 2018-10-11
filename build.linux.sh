#!/bin/sh

TARGET="linux"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="gcc"
CFLAGS="-Wall -O3 -std=gnu11 `sdl2-config --cflags`"
INCLUDE_PATHS="-I $DIR/bi-core/include -I $DIR/bi-ext/include"
LDFLAGS="-lbiext -lbi -lGLEW -lGL -lm -lSDL2_image `sdl2-config --libs`"
LIB_PATHS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET"

# build bi-core
if [ ! -e $DIR/bi-core/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-core ]; then
    git clone https://github.com/bismite/bi-core.git $DIR/bi-core
  fi
  (cd $DIR/bi-core; make -f Makefile.$TARGET.mk clean all )
  if [ $? != 0 ]; then exit 1; fi
fi

# build bi-ext
if [ ! -e $DIR/bi-ext/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-ext ]; then
    git clone https://github.com/bismite/bi-ext.git $DIR/bi-ext
  fi
  (cd $DIR/bi-ext; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../bi-core/include" )
  if [ $? != 0 ]; then exit 1; fi
fi

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -o $DIR/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS
  if [ $? != 0 ]; then exit 1; fi
done