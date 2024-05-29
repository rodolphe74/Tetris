#!/bin/sh
set -e
if test "$CONFIGURATION" = "Debug"; then :
  cd /Users/rodoc/develop/Tetris/xcode
  /opt/homebrew/bin/python3.12 /Users/rodoc/develop/vcpkg/scripts/buildsystems/osx/applocal.py /Users/rodoc/develop/Tetris/xcode/Debug/tetris.app/Contents/MacOS/tetris /Users/rodoc/develop/vcpkg/installed/arm64-osx/debug
fi
if test "$CONFIGURATION" = "Release"; then :
  cd /Users/rodoc/develop/Tetris/xcode
  /opt/homebrew/bin/python3.12 /Users/rodoc/develop/vcpkg/scripts/buildsystems/osx/applocal.py /Users/rodoc/develop/Tetris/xcode/Release/tetris.app/Contents/MacOS/tetris /Users/rodoc/develop/vcpkg/installed/arm64-osx
fi
if test "$CONFIGURATION" = "MinSizeRel"; then :
  cd /Users/rodoc/develop/Tetris/xcode
  /opt/homebrew/bin/python3.12 /Users/rodoc/develop/vcpkg/scripts/buildsystems/osx/applocal.py /Users/rodoc/develop/Tetris/xcode/MinSizeRel/tetris.app/Contents/MacOS/tetris /Users/rodoc/develop/vcpkg/installed/arm64-osx
fi
if test "$CONFIGURATION" = "RelWithDebInfo"; then :
  cd /Users/rodoc/develop/Tetris/xcode
  /opt/homebrew/bin/python3.12 /Users/rodoc/develop/vcpkg/scripts/buildsystems/osx/applocal.py /Users/rodoc/develop/Tetris/xcode/RelWithDebInfo/tetris.app/Contents/MacOS/tetris /Users/rodoc/develop/vcpkg/installed/arm64-osx
fi

