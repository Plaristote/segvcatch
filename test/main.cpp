#include "exceptdefs.h"
#include "utils.hpp"
#include <gtest/gtest.h>
#include <iostream>
#include <segvcatch.h>
#include <stdexcept>
#include <string>

using namespace std;

void handle_segv() { throw std::runtime_error("My SEGV"); }

void handle_fpe() { throw std::runtime_error("My FPE"); }

TEST(Segvcatch, InitSegv) {
    segvcatch::init_segv(nullptr);
    segvcatch::init_fpe(nullptr);

    try {
        /*
         * In my tests with ARM, this try-catch block is optimized away by the
         * compiler without some other code that can throw an exception.
         */
        std::cout << "Do not optimize away the try-catch block!" << std::endl;

        segvcatch::donot_optimize_away();
        test::causes_segv();
    } catch (const segvcatch::hardware_exception &e) {
        std::cerr << "Exception catched : " << e.what()
                  << " where: " << e.info.addr << std::endl;
    } catch (const std::exception &e) {
        std::cerr << "Exception catched : " << e.what() << std::endl;
    }

    try {
#ifndef __ARM_ARCH // TODO
        test::divide_by_zero();
#endif
    } catch (const segvcatch::hardware_exception &e) {
        std::cerr << "Exception catched : " << e.what()
                  << " where: " << e.info.addr << std::endl;
    } catch (std::exception &e) {
        std::cerr << "Exception catched : " << e.what() << std::endl;
    }

    std::cout << "We are living yet!" << std::endl;
}
