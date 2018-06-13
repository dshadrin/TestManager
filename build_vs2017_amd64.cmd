@echo off
set BOOST_INCLUDEDIR=d:\usr\include\boost-1_67
set BOOST_LIBRARYDIR=d:\usr\lib
set PostgreSQL_LIBRARY=e:\PostgreSQL\9.6\lib
set PostgreSQL_INCLUDE_DIR=e:\PostgreSQL\9.6\include
rmdir /S /Q build
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017 Win64"
pause
