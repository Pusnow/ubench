
#include "common.h"

void ubench_test(void) {
    ubench_uint_t prev, cycles;
    int i;
    prev = ubench_get_cycles();
    for (i = 0; i < 1000; ++i) {
        cycles = ubench_get_cycles();
        UB_EXPECT(cycles >= prev);
        prev = cycles;
    }
}