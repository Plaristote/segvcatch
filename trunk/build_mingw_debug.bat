mkdir debug
cd debug
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug ..\src
mingw32-make
cd ..
pause