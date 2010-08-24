mkdir vs2008_release
cd vs2008_release
cmake -G "Visual Studio 9 2008" -DCMAKE_BUILD_TYPE=Release ..\src
rem make
cd ..
pause