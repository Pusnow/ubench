#include "common.h"

ubench_uint_t values[] = {
    43256184, 95192806, 3270895,  91202119, 95206400, 68841354, 28840743,
    90199941, 6545494,  46519111, 9776299,  10151305, 16450599, 94646406,
    63502117, 23370966, 35345217, 14523679, 1646259,  25930884, 70536203,
    54407826, 7857181,  98350342, 67009617, 13207455, 69313736, 72259771,
    50940439, 4448831,  5426305,  62463665, 8958145,  54311016, 21739453,
    96632118, 21122411, 66062385, 80031438, 72057867, 65691976, 82770604,
    50332848, 88267191, 79217920, 92681913, 49568829, 81103759, 1675719,
    94089487, 26585608, 6328820,  76430296, 28953316,
};

const char* exp_str = "[hist-range-basic-test] [1477594 - 2216391): 2\n"
                      "[hist-range-basic-test] [2216391 - 3324586): 1\n"
                      "[hist-range-basic-test] [3324586 - 4986879): 1\n"
                      "[hist-range-basic-test] [4986879 - 7480318): 3\n"
                      "[hist-range-basic-test] [7480318 - 11220477): 4\n"
                      "[hist-range-basic-test] [11220477 - 16830715): 3\n"
                      "[hist-range-basic-test] [16830715 - 25246072): 3\n"
                      "[hist-range-basic-test] [25246072 - 37869108): 5\n"
                      "[hist-range-basic-test] [37869108 - 56803662): 7\n"
                      "[hist-range-basic-test] [56803662 - 85205493): 15\n"
                      "[hist-range-basic-test] [85205493 - 127808239): 10\n";

void ubench_test(void) {
    ubench_uint_t total = 54;
    int i;
    ubench_hist_range_t hist;
    ubench_hist_range_init(&hist, 1000, 3, 2);

    for (i = 0; i < 54; ++i) {
        ubench_hist_range_add(&hist, values[i]);
    }
    UB_EXPECT_EQ(hist.total, total);
    UB_EXPECT_EQ(hist.counts[19], (ubench_uint_t)2);
    UB_EXPECT_EQ(hist.counts[20], (ubench_uint_t)1);
    UB_EXPECT_EQ(hist.counts[21], (ubench_uint_t)1);
    UB_EXPECT_EQ(hist.counts[22], (ubench_uint_t)3);
    UB_EXPECT_EQ(hist.counts[23], (ubench_uint_t)4);
    UB_EXPECT_EQ(hist.counts[24], (ubench_uint_t)3);
    UB_EXPECT_EQ(hist.counts[25], (ubench_uint_t)3);
    UB_EXPECT_EQ(hist.counts[26], (ubench_uint_t)5);
    UB_EXPECT_EQ(hist.counts[27], (ubench_uint_t)7);
    UB_EXPECT_EQ(hist.counts[28], (ubench_uint_t)15);
    UB_EXPECT_EQ(hist.counts[29], (ubench_uint_t)10);

    ubench_hist_range_print("hist-range-basic-test", &hist);
    UB_EXPECT(strcmp(exp_str, ubench_debug_printf_buff.buff) == 0);
}