#!/bin/sh

TARGET="linux"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

BI_CORE_DIR=$DIR/bi-core
BI_EXT_DIR=$DIR/bi-ext

CC="clang"
CFLAGS="-Wall -O3 -std=gnu11 `sdl2-config --cflags`"
INCLUDE_PATHS="-I $BI_CORE_DIR/include -I $BI_EXT_DIR/include"
LDFLAGS="-lbiext -lbi -lGLEW -lGL -lm -lSDL2_image `sdl2-config --libs`"
LIB_PATHS="-L $BI_CORE_DIR/build/$TARGET -L $BI_EXT_DIR/build/$TARGET"

mkdir -p $DIR/bin
mkdir -p $DIR/lib
mkdir -p $DIR/include

if [ ! -e $BI_CORE_DIR ]; then
  if [ -z $BI_CORE ]; then
    git clone https://github.com/bismite/bi-core.git $BI_CORE_DIR
  else
    cp -R $BI_CORE $DIR
  fi
fi

if [ ! -e $BI_EXT_DIR ]; then
  if [ -z $BI_EXT ]; then
    git clone https://github.com/bismite/bi-ext.git $BI_EXT_DIR
  else
    cp -R $BI_EXT $DIR
  fi
fi

# build bi-core
(cd $BI_CORE_DIR; make -f Makefile.$TARGET.mk clean all )
(cd $BI_EXT_DIR; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../bi-core/include" )

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -o $DIR/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS
  if [ $? != 0 ]; then exit 1; fi
done
