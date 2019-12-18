#!/bin/sh

TARGET="macos"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="/usr/bin/clang"
FRAMEWORKS="-F $HOME/Library/Frameworks -framework OpenGL -framework SDL2 -framework SDL2_image"
CFLAGS="-Wall -O3 -I $DIR/bi-core/include -I $DIR/bi-ext/include -I $DIR/include"
LIBS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET -L $DIR/lib -lbiext -lbi -lglew"

BI_CORE_DIR=$DIR/bi-core
BI_EXT_DIR=$DIR/bi-ext

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

sh ./scripts/$TARGET/install_sdl.sh $DIR
sh ./scripts/$TARGET/install_glew.sh $DIR $DIR
(cd $BI_CORE_DIR; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../include" )
(cd $DIR/bi-ext; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../include -I ../bi-core/include" )


#
# build samples
#

# for SRC in "src/action.c" "src/timer.c" "src/label.c" "src/layer-particle.c" "src/layer-spotlight.c"; do
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -Wall -o $DIR/bin/$NAME $SRC $CFLAGS $LIBS $FRAMEWORKS
  if [ $? != 0 ]; then exit 1; fi
  otool -L $DIR/bin/$NAME
done
