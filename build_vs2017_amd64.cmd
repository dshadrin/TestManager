@echo off
::set PostgreSQL_ROOT=d:\PostgreSQL\10
set PostgreSQL_ROOT=e:\PostgreSQL\9.6
echo PostgreSQL ROOT DIR: %PostgreSQL_ROOT%
set BOOST_INCLUDEDIR=d:\usr\include\boost-1_67
set BOOST_LIBRARYDIR=d:\usr\lib
rmdir /S /Q build
mkdir build
cd build
cmake .. -G "Visual Studio 15 2017 Win64"
pause
