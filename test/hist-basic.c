
#include "common.h"

ubench_uint_t values[] = {
    43256184,
    95192806,
    3270895,
    91202119,
    95206400,
    68841354,
    28840743,
    90199941,
    6545494,
    46519111,
    9776299,
    10151305,
    16450599,
    94646406,
    63502117,
    23370966,
    35345217,
    14523679,
    1646259,
    25930884,
    70536203,
    54407826,
    7857181,
    98350342,
    67009617,
    13207455,
    69313736,
    72259771,
    50940439,
    4448831,
    5426305,
    62463665,
    8958145,
    54311016,
    21739453,
    96632118,
    21122411,
    66062385,
    80031438,
    72057867,
    65691976,
    82770604,
    50332848,
    88267191,
    79217920,
    92681913,
    49568829,
    81103759,
    1675719,
    94089487,
    26585608,
    6328820,
    76430296,
    28953316,
};

const char *exp_str =
    "[hist-basic-test] [1048575 - 2097151): 2\n"
    "[hist-basic-test] [2097151 - 4194303): 1\n"
    "[hist-basic-test] [4194303 - 8388607): 5\n"
    "[hist-basic-test] [8388607 - 16777215): 6\n"
    "[hist-basic-test] [16777215 - 33554431): 7\n"
    "[hist-basic-test] [33554431 - 67108863): 13\n"
    "[hist-basic-test] [67108863 - 134217727): 20\n";

void ubench_test(void)
{
    ubench_uint_t total = 54;
    int i;
    ubench_hist_t hist;
    ubench_hist_init(&hist);

    for (i = 0; i < 54; ++i)
    {
        ubench_hist_add(&hist, values[i]);
    }
    UB_EXPECT_EQ(hist.total, total);
    UB_EXPECT_EQ(hist.counts[20], (ubench_uint_t)2);
    UB_EXPECT_EQ(hist.counts[21], (ubench_uint_t)1);
    UB_EXPECT_EQ(hist.counts[22], (ubench_uint_t)5);
    UB_EXPECT_EQ(hist.counts[23], (ubench_uint_t)6);
    UB_EXPECT_EQ(hist.counts[24], (ubench_uint_t)7);
    UB_EXPECT_EQ(hist.counts[25], (ubench_uint_t)13);
    UB_EXPECT_EQ(hist.counts[26], (ubench_uint_t)20);

    ubench_hist_print("hist-basic-test", &hist);
    UB_EXPECT(strcmp(exp_str, ubench_debug_printf_buff.buff) == 0);
}