/*! \mainpage SEGVCATCH library documentation

\section about About

This is a crossplatform C++ library  designed to convert a hardware exceptions, such as segmentation fault, or floating point errors, into a software language exceptions, which can be handled later with a try/catch construction.

Other words, it's a crossplatform structured exception handling (SEH).

For example, this code is working fine:

\code
try
{
    *(int*) 0 = 0;
}
catch (std::exception& e)
{
    std::cerr << "Exception catched : " << e.what() << std::endl;
}
\endcode

\section install Installation

<a href="http://www.cmake.org">CMake</a> is required to build project.

Get the sources from http://code.google.com/p/segvcatch. Unpack if needed.

On Linux/Unix you have to:
\verbatim
$ cmake .
$ make
$ sudo make install
\endverbatim

\section using Using

You need to link this library to your project. Then call initializers segvcatch::init_segv(), segvcatch::init_fpe() in a beginning of main() function.

It's a short example:
\code
#include <iostream>
#include <stdexcept>
#include <segvcatch.h>

using namespace std;

int main(int argc, char *argv[])
{
    segvcatch::init_segv();
    segvcatch::init_fpe();

    try
    {
        *(int*) 0 = 0;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception catched : " << e.what() << std::endl;
    }

    try
    {
        int v = 0;
        std::cout << 10 / v << std::endl;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception catched : " << e.what() << std::endl;
    }

    std::cout << "We are living yet!" << std::endl;

    return 0;
}
\endcode



\author Navrocky Vladislav
*/