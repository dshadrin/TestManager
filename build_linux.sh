@echo off
rm -fr build
mkdir build
cd build
cmake .. -G"Unix Makefiles" -DCMAKE_BUILD_TYPE:STRING="Release"
pause
make install

