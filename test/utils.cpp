#include "utils.hpp"
#include <cstdio>

namespace test {

void causes_segv() {
    int *p = nullptr;
    *p = 0;
}

void divide_by_zero() {
    int v = 0;
    int r = 10 / v;
    std::printf("%d\n", r);
}

} // namespace test
