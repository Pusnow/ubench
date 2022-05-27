
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

void ubench_test(void)
{
    ubench_uint_t sum = 2685253268;
    ubench_uint_t total = 54;
    int i;

    ubench_stat_t stat;
    ubench_stat_init(&stat);

    for (i = 0; i < 54; ++i)
    {
        ubench_stat_add(&stat, values[i]);
    }
    UB_EXPECT_EQ(stat.total, total);
    UB_EXPECT_EQ(stat.sum, sum);

    ubench_stat_print("stat-basic-test", &stat);
    UB_EXPECT(strcmp("[stat-basic-test] avg: 49726912 sum: 2685253268 total: 54\n", ubench_debug_printf_buff.buff) == 0);
}
