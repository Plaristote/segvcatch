mkdir release
cd release
cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release ..\src
mingw32-make
cd ..
pause