#!/bin/sh

echo "Install GLEW"

DIR=$1
INSTALL_DIR=$2
URL=https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz

mkdir -p $INSTALL_DIR/lib
mkdir -p $INSTALL_DIR/include


if [ ! -e $DIR/glew-2.1.0/lib/libGLEW.a ]; then
  if [ ! -e $DIR/glew-2.1.0 ]; then
    if [ ! -e $DIR/glew-2.1.0.tgz ]; then
      echo $URL
      curl --progress-bar -S -L -o $DIR/glew-2.1.0.tgz $URL
    fi
    tar zxf $DIR/glew-2.1.0.tgz -C $DIR
  fi
  (cd $DIR/glew-2.1.0; make glew.lib.static)
fi

cp $DIR/glew-2.1.0/lib/* $INSTALL_DIR/lib
cp -R $DIR/glew-2.1.0/include/* $INSTALL_DIR/include
