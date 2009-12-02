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
