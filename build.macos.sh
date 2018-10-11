#!/bin/sh

TARGET="macos"
DIR="build/$TARGET"
SOURCES=`find src -name "*.c"`

CC="/usr/bin/clang"
FRAMEWORKS="-F $HOME/Library/Frameworks -framework OpenGL -framework SDL2 -framework SDL2_image"
CFLAGS="-Wall -O3 -I $DIR/bi-core/include -I $DIR/bi-ext/include -I $DIR/glew-2.1.0/include"
LIBS="-L $DIR/bi-core/build/$TARGET -L $DIR/bi-ext/build/$TARGET -L $DIR/glew-2.1.0/lib -lbiext -lbi -lglew"

mkdir -p $DIR

# build GLEW
if [ ! -e $DIR/glew-2.1.0/lib/libGLEW.a ]; then
  if [ ! -e $DIR/glew-2.1.0 ]; then
    if [ ! -e $DIR/glew-2.1.0.tgz ]; then
      curl -sS -L -o $DIR/glew-2.1.0.tgz https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz
    fi
    tar zxf $DIR/glew-2.1.0.tgz -C $DIR
  fi
  (cd $DIR/glew-2.1.0; make glew.lib.static)
fi

# build bi-core
if [ ! -e $DIR/bi-core/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-core ]; then
    git clone https://github.com/bismite/bi-core.git $DIR/bi-core
    # cp -R ../bi-core $DIR
  fi
  (cd $DIR/bi-core; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../glew-2.1.0/include" )
  if [ $? != 0 ]; then exit 1; fi
fi

# build bi-ext
if [ ! -e $DIR/bi-ext/build/$TARGET/libbi.a ]; then
  if [ ! -e $DIR/bi-ext ]; then
    # git clone
    # cp -R ../bi-ext $DIR
    git clone https://github.com/bismite/bi-ext.git $DIR/bi-ext
  fi
  (cd $DIR/bi-ext; make -f Makefile.$TARGET.mk clean all INCLUDE_PATHS="-I ../glew-2.1.0/include -I ../bi-core/include" )
  if [ $? != 0 ]; then exit 1; fi
fi

#
# build samples
#
for SRC in $SOURCES; do
  echo $SRC
  NAME=`basename $SRC .c`
  $CC -o $DIR/$NAME $SRC $CFLAGS $LIBS $FRAMEWORKS
  if [ $? != 0 ]; then exit 1; fi
  otool -L build/macos/$NAME
done
