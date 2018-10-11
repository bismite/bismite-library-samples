#!/bin/sh

TARGET="mingw"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="x86_64-w64-mingw32-gcc"
CFLAGS="-Wall -O3 -std=gnu11 `/usr/local/x86_64-w64-mingw32/bin/sdl2-config --cflags`"
INCLUDE_PATHS="-I /usr/local/x86_64-w64-mingw32/include -I $DIR/bi-core/include -I $DIR/bi-ext/include"
LDFLAGS="-lbiext -lbi -lglew32 -lopengl32 -lSDL2_image `/usr/local/x86_64-w64-mingw32/bin/sdl2-config --libs`"
LIB_PATHS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET -L /usr/local/x86_64-w64-mingw32/lib"


mkdir -p $DIR
cp /usr/local/x86_64-w64-mingw32/bin/*.dll build/$TARGET/

# build bi-core
if [ ! -e $DIR/bi-core/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-core ]; then
    # git clone
    cp -R ../bi-core $DIR
  fi
  (cd $DIR/bi-core; make -f Makefile.$TARGET.mk clean all )
  if [ $? != 0 ]; then exit 1; fi
fi

# build bi-ext
if [ ! -e $DIR/bi-ext/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-ext ]; then
    # git clone
    cp -R ../bi-ext $DIR
  fi
  (cd $DIR/bi-ext; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../bi-core/include -I /usr/local/x86_64-w64-mingw32/include" )
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
