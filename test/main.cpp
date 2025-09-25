#include <iostream>
#include <stdexcept>
#include <string>

#include <segvcatch.h>

using namespace std;

void handle_segv()
{
    throw std::runtime_error("My SEGV");
}

void handle_fpe()
{
    throw std::runtime_error("My FPE");
}

int main(int argc, char *argv[])
{
    segvcatch::init_segv(&handle_segv);
    segvcatch::init_fpe(&handle_fpe);

    try
    {
        /*
        * In my tests with ARM, this try-catch block is optimized away by the compiler without some other code that can throw an exception.
        */
        std::cout << "Do not optimize away the try-catch block!" << std::endl;
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
