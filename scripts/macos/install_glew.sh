#!/bin/bash

if [ ! -d "build/macos/include/GL" ]; then
  mkdir -p build/download
  mkdir -p build/macos/lib
  mkdir -p build/macos/include

  GLEW_MD5=$(md5 -q build/download/glew-2.1.0.tgz)
  if [ $GLEW_MD5 != "b2ab12331033ddfaa50dc39345343980" ]; then
    GLEW_URL="https://github.com/nigels-com/glew/releases/download/glew-2.1.0/glew-2.1.0.tgz"
    echo "Download ${GLEW_URL}"
    curl --progress-bar -S -L -o build/download/glew-2.1.0.tgz $GLEW_URL
  fi

  tar zxf build/download/glew-2.1.0.tgz -C build/macos/
  (cd build/macos/glew-2.1.0; make glew.lib.static)

  # copy glew
  cp build/macos/glew-2.1.0/lib/* build/macos/lib
  cp -R build/macos/glew-2.1.0/include/* build/macos/include
fi
