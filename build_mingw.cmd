@echo off
set PostgreSQL_ROOT=d:\PostgreSQL\10
::set PostgreSQL_ROOT=e:\PostgreSQL\9.6
set PATH=d:\Qt\Tools\mingw530_32\bin;%PATH%
set BOOST_LIBRARYDIR=%BOOST_LIBRARYDIR%32
rmdir /S /Q build
mkdir build
cd build
cmake .. -G"MinGW Makefiles" -DCMAKE_BUILD_TYPE:STRING="Release"
pause
mingw32-make install

