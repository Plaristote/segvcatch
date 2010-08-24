mkdir vs2008_debug
cd vs2008_debug
cmake -G "Visual Studio 9 2008" -DCMAKE_BUILD_TYPE=Debug ..\src
rem make
cd ..
pause