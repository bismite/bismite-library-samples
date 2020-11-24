#!/bin/sh

DIR=$1
BI_CORE_DIR=$DIR/bismite-library-core
BI_EXT_DIR=$DIR/bismite-library-ext

if [ ! -d $BI_CORE_DIR ]; then
  if [ -z $BI_CORE ]; then
    echo "Copy bismite-library-core from git"
    git clone https://github.com/bismite/bismite-library-core.git $BI_CORE_DIR
  else
    echo "Copy bismite-library-core from $BI_CORE"
    cp -r $BI_CORE $DIR
    rm -r $BI_CORE_DIR/build
  fi
fi

if [ ! -d $BI_EXT_DIR ]; then
  if [ -z $BI_EXT ]; then
    echo "Copy bismite-library-ext from git"
    git clone https://github.com/bismite/bismite-library-ext.git $BI_EXT_DIR
  else
    echo "Copy bismite-library-ext from $BI_EXT"
    cp -R $BI_EXT $DIR
    rm -r $BI_EXT_DIR/build
  fi
fi
