@echo off
set PATH=d:\Qt\Tools\mingw530_32\bin;%PATH%
set BOOST_LIBRARYDIR=d:\usr\lib32
rmdir /S /Q build
mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING="Release"
pause
mingw32-make install

