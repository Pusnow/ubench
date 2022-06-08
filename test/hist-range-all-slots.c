#include "common.h"

void ubench_test(void) {
    ubench_uint_t i, min, n, d, bin;
    ubench_hist_range_t hist;

    n = 3;
    d = 2;
    min = 1000;

    ubench_hist_range_init(&hist, min, n, d);
    bin = min;
    for (i = 0; i < 63; ++i) {
        ubench_hist_range_add(&hist, bin - 1);
        ubench_hist_range_add(&hist, bin);
        ubench_hist_range_add(&hist, bin + 1);
        bin *= n;
        bin /= d;
    }
    UB_EXPECT_EQ(hist.counts[0], (ubench_uint_t)1);

    for (i = 1; i < 63; ++i) {
        UB_EXPECT_EQ(hist.counts[i], (ubench_uint_t)3);
    }
    UB_EXPECT_EQ(hist.counts[63], (ubench_uint_t)2);
    ubench_hist_range_print("hist-range-basic-test", &hist);
}