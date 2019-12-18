#!/bin/sh

mkdir -p doc

SOURCES=`find src -name "*.c"`
for SRC in $SOURCES; do
  NAME=`basename $SRC .c`
  (sleep 3;gnome-screenshot -w -f doc/$NAME.png) & timeout 5 ./build/linux/$NAME
done
