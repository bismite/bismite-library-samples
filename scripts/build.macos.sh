#!/bin/bash

TARGET="macos"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="/usr/bin/clang"
ARCHFLAG="-arch x86_64 -arch arm64"
FRAMEWORKS="-framework OpenGL"
CFLAGS="-Wall -Os -I $DIR/include -I $DIR/include/SDL2 ${ARCHFLAG}"
LIBS="-L $DIR/lib -lbismite-ext -lbismite-core -lSDL2 -lSDL2_image -lSDL2_mixer"

BI_CORE_DIR=$DIR/bismite-library-core
BI_EXT_DIR=$DIR/bismite-library-ext

mkdir -p $DIR/bin
mkdir -p $DIR/lib
mkdir -p $DIR/include

./scripts/$TARGET/install_sdl.sh $DIR

sh ./scripts/copy-bismite-libraries.sh $DIR

(cd $BI_CORE_DIR; make -f Makefile.$TARGET.mk all INCLUDE_PATHS="-I ../include -I ../include/SDL2" CFLAGS="-Wall -Os ${ARCHFLAG}" )
if [ $? != 0 ]; then exit 1; fi
cp $BI_CORE_DIR/build/$TARGET/*.a $DIR/lib/
cp -r $BI_CORE_DIR/include/bi $DIR/include/

(cd $BI_EXT_DIR; make -f Makefile.$TARGET.mk all INCLUDE_PATHS="-I ../include -I ../include/SDL2"  CFLAGS="-Wall -Os ${ARCHFLAG}" )
if [ $? != 0 ]; then exit 1; fi
cp $BI_EXT_DIR/build/$TARGET/*.a $DIR/lib/
cp -r $BI_EXT_DIR/include/bi $DIR/include/

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -Wall -c $SRC -o $DIR/bin/$NAME.o $CFLAGS
  $CC -Wall $DIR/bin/$NAME.o -o $DIR/bin/$NAME $LIBS $FRAMEWORKS $ARCHFLAG
  install_name_tool -add_rpath @executable_path/../lib $DIR/bin/$NAME
  if [ $? != 0 ]; then exit 1; fi
done
