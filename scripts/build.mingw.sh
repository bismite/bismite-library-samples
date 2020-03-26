#!/bin/sh

TARGET="mingw"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

SDL_PREFIX=/usr/local/x86_64-w64-mingw32
CC="x86_64-w64-mingw32-gcc"
CFLAGS="-Wall -O3 -std=gnu11 `${SDL_PREFIX}/bin/sdl2-config --cflags`"
INCLUDE_PATHS="-I build/mingw/include -I $DIR/bi-core/include -I $DIR/bi-ext/include"
LDFLAGS="-lbiext -lbi -lglew32 -lopengl32 -lSDL2_image `${SDL_PREFIX}/bin/sdl2-config --libs`"
LIB_PATHS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET -L build/mingw/lib"

mkdir -p $DIR/bin
mkdir -p $DIR/lib
mkdir -p $DIR/include

sh ./scripts/$TARGET/install_glew.sh $DIR

cp /usr/local/x86_64-w64-mingw32/bin/*.dll build/$TARGET/

# build bi-core
if [ -z $BI_CORE ]; then
  git clone https://github.com/bismite/bi-core.git $BI_CORE_DIR
else
  cp -R $BI_CORE $DIR
fi
(cd $DIR/bi-core; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../include -I ${SDL_PREFIX}/include" )
if [ $? != 0 ]; then exit 1; fi


# build bi-ext
if [ -z $BI_EXT ]; then
  git clone https://github.com/bismite/bi-ext.git $BI_EXT_DIR
else
  cp -R $BI_EXT $DIR
fi
(cd $DIR/bi-ext; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../include -I ${SDL_PREFIX}/include -I ../bi-core/include" )
if [ $? != 0 ]; then exit 1; fi

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  echo "$CC -o $DIR/bin/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS"
  $CC -o $DIR/bin/$NAME $SRC $CFLAGS $INCLUDE_PATHS $LIB_PATHS $LDFLAGS
  if [ $? != 0 ]; then exit 1; fi
done

cp ${SDL_PREFIX}/bin/*.dll build/mingw/bin
