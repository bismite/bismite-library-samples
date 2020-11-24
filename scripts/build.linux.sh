#!/bin/sh

TARGET="linux"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

BI_CORE_DIR=$DIR/bismite-library-core
BI_EXT_DIR=$DIR/bismite-library-ext

CC="clang"
CFLAGS="-Wall -O3 -std=gnu11 `sdl2-config --cflags`"
INCLUDE_PATHS="-I $DIR/include"
LDFLAGS="-lbismite-ext -lbismite-core -lGLEW -lGL -lm -lSDL2_image `sdl2-config --libs`"
LIB_PATHS="-L $DIR/lib"

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
  $CC -o $DIR/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS
  if [ $? != 0 ]; then exit 1; fi
done
