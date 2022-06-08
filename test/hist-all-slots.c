
#include "common.h"

const char* exp_str =
    "[hist-all-slots-test] [0 - 1): 1\n"
    "[hist-all-slots-test] [1 - 3): 4\n"
    "[hist-all-slots-test] [3 - 7): 4\n"
    "[hist-all-slots-test] [7 - 15): 3\n"
    "[hist-all-slots-test] [15 - 31): 3\n"
    "[hist-all-slots-test] [31 - 63): 3\n"
    "[hist-all-slots-test] [63 - 127): 3\n"
    "[hist-all-slots-test] [127 - 255): 3\n"
    "[hist-all-slots-test] [255 - 511): 3\n"
    "[hist-all-slots-test] [511 - 1023): 3\n"
    "[hist-all-slots-test] [1023 - 2047): 3\n"
    "[hist-all-slots-test] [2047 - 4095): 3\n"
    "[hist-all-slots-test] [4095 - 8191): 3\n"
    "[hist-all-slots-test] [8191 - 16383): 3\n"
    "[hist-all-slots-test] [16383 - 32767): 3\n"
    "[hist-all-slots-test] [32767 - 65535): 3\n"
    "[hist-all-slots-test] [65535 - 131071): 3\n"
    "[hist-all-slots-test] [131071 - 262143): 3\n"
    "[hist-all-slots-test] [262143 - 524287): 3\n"
    "[hist-all-slots-test] [524287 - 1048575): 3\n"
    "[hist-all-slots-test] [1048575 - 2097151): 3\n"
    "[hist-all-slots-test] [2097151 - 4194303): 3\n"
    "[hist-all-slots-test] [4194303 - 8388607): 3\n"
    "[hist-all-slots-test] [8388607 - 16777215): 3\n"
    "[hist-all-slots-test] [16777215 - 33554431): 3\n"
    "[hist-all-slots-test] [33554431 - 67108863): 3\n"
    "[hist-all-slots-test] [67108863 - 134217727): 3\n"
    "[hist-all-slots-test] [134217727 - 268435455): 3\n"
    "[hist-all-slots-test] [268435455 - 536870911): 3\n"
    "[hist-all-slots-test] [536870911 - 1073741823): 3\n"
    "[hist-all-slots-test] [1073741823 - 2147483647): 3\n"
    "[hist-all-slots-test] [2147483647 - 4294967295): 3\n"
    "[hist-all-slots-test] [4294967295 - 8589934591): 3\n"
    "[hist-all-slots-test] [8589934591 - 17179869183): 3\n"
    "[hist-all-slots-test] [17179869183 - 34359738367): 3\n"
    "[hist-all-slots-test] [34359738367 - 68719476735): 3\n"
    "[hist-all-slots-test] [68719476735 - 137438953471): 3\n"
    "[hist-all-slots-test] [137438953471 - 274877906943): 3\n"
    "[hist-all-slots-test] [274877906943 - 549755813887): 3\n"
    "[hist-all-slots-test] [549755813887 - 1099511627775): 3\n"
    "[hist-all-slots-test] [1099511627775 - 2199023255551): 3\n"
    "[hist-all-slots-test] [2199023255551 - 4398046511103): 3\n"
    "[hist-all-slots-test] [4398046511103 - 8796093022207): 3\n"
    "[hist-all-slots-test] [8796093022207 - 17592186044415): 3\n"
    "[hist-all-slots-test] [17592186044415 - 35184372088831): 3\n"
    "[hist-all-slots-test] [35184372088831 - 70368744177663): 3\n"
    "[hist-all-slots-test] [70368744177663 - 140737488355327): 3\n"
    "[hist-all-slots-test] [140737488355327 - 281474976710655): 3\n"
    "[hist-all-slots-test] [281474976710655 - 562949953421311): 3\n"
    "[hist-all-slots-test] [562949953421311 - 1125899906842623): 3\n"
    "[hist-all-slots-test] [1125899906842623 - 2251799813685247): 3\n"
    "[hist-all-slots-test] [2251799813685247 - 4503599627370495): 3\n"
    "[hist-all-slots-test] [4503599627370495 - 9007199254740991): 3\n"
    "[hist-all-slots-test] [9007199254740991 - 18014398509481983): 3\n"
    "[hist-all-slots-test] [18014398509481983 - 36028797018963967): 3\n"
    "[hist-all-slots-test] [36028797018963967 - 72057594037927935): 3\n"
    "[hist-all-slots-test] [72057594037927935 - 144115188075855871): 3\n"
    "[hist-all-slots-test] [144115188075855871 - 288230376151711743): 3\n"
    "[hist-all-slots-test] [288230376151711743 - 576460752303423487): 3\n"
    "[hist-all-slots-test] [576460752303423487 - 1152921504606846975): 3\n"
    "[hist-all-slots-test] [1152921504606846975 - 2305843009213693951): 3\n"
    "[hist-all-slots-test] [2305843009213693951 - 4611686018427387903): 3\n"
    "[hist-all-slots-test] [4611686018427387903 - 9223372036854775807): 3\n"
    "[hist-all-slots-test] [9223372036854775807 - ): 3\n";

void ubench_test(void) {
    ubench_uint_t i;
    ubench_hist_t hist;
    ubench_hist_init(&hist);

    for (i = 0; i < 64; ++i) {
        ubench_hist_add(&hist, ((ubench_uint_t)1 << i) - 1);
        ubench_hist_add(&hist, (ubench_uint_t)1 << i);
        ubench_hist_add(&hist, ((ubench_uint_t)1 << i) + 1);
    }

    ubench_hist_print("hist-all-slots-test", &hist);
    UB_EXPECT(strcmp(exp_str, ubench_debug_printf_buff.buff) == 0);
}