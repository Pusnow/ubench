#ifndef __UBENCH_TESTS_COMMON_H
#define __UBENCH_TESTS_COMMON_H

#define UBENCH_TEST_OK 0
#define UBENCH_TEST_FAIL 1

void ubench_test(void);

#ifndef UBENCH_DEBUG_PRINTF_BUFFER_SIZE
#define UBENCH_DEBUG_PRINTF_BUFFER_SIZE 8192
#endif

#ifndef UBENCH_TEST_NAME
#define UBENCH_TEST_NAME_STR ""
#else
#define UBENCH_TEST_NAME_QUOTE(X) #X
#define UBENCH_TEST_NAME_STRF(X) UBENCH_TEST_NAME_QUOTE(X) \
": "
#define UBENCH_TEST_NAME_STR UBENCH_TEST_NAME_STRF(UBENCH_TEST_NAME)
#endif
struct ubench_debug_printf_s
{
    int cur;
    char buff[UBENCH_DEBUG_PRINTF_BUFFER_SIZE + 1]; // for safety
};

struct ubench_debug_printf_s ubench_debug_printf_buff;

#if defined(__KERNEL__)
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/printk.h>

static int ubench_kmod_return_code;
static int __init ubench_kmod_init(void)
{
    ubench_kmod_return_code = UBENCH_TEST_OK;
    ubench_debug_printf_buff.cur = 0;
    pr_info(UBENCH_TEST_NAME_STR "Initializing kmod\n");
    ubench_test();
    pr_info(UBENCH_TEST_NAME_STR "Test Result: %s\n", ubench_kmod_return_code == UBENCH_TEST_OK ? "OK" : "Fail");
    return 0;
}
static void __exit ubench_kmod_exit(void)
{
    pr_info(UBENCH_TEST_NAME_STR "Exiting kmod\n");
}

module_init(ubench_kmod_init);
module_exit(ubench_kmod_exit);
MODULE_LICENSE("GPLv2");

#define UB_ON_FAIL                                  \
    do                                              \
    {                                               \
        ubench_kmod_return_code = UBENCH_TEST_FAIL; \
    } while (0);

#else
#include <stdlib.h>
#include <string.h>

int main()
{
    ubench_debug_printf_buff.cur = 0;
    ubench_test();
    return 0;
}

#define UB_ON_FAIL              \
    do                          \
    {                           \
        exit(UBENCH_TEST_FAIL); \
    } while (0);
#endif

#define UBENCH_PRINTF(...)                                                                                                                                                \
    do                                                                                                                                                                    \
    {                                                                                                                                                                     \
        int printf_len;                                                                                                                                                   \
        UBENCH_PRINTF_INNER(UBENCH_TEST_NAME_STR __VA_ARGS__);                                                                                                            \
        printf_len = snprintf(ubench_debug_printf_buff.buff + ubench_debug_printf_buff.cur, UBENCH_DEBUG_PRINTF_BUFFER_SIZE - ubench_debug_printf_buff.cur, __VA_ARGS__); \
        ubench_debug_printf_buff.cur += printf_len;                                                                                                                       \
    } while (0)

#include "../ubench.h"

#define UB_EXPECT(x)                                             \
    if (!(x))                                                    \
    {                                                            \
        UBENCH_PRINTF("Test fail (\"" #x "\"): return false\n"); \
        UB_ON_FAIL;                                              \
    }
#define UB_EXPECT_EQ(x, y)                                                             \
    if ((x) != (y))                                                                    \
    {                                                                                  \
        UBENCH_PRINTF_INNER("Test fail (\"" #x " == " #y "\"): %lld vs %lld\n", x, y); \
        UB_ON_FAIL;                                                                    \
    }
#define UB_EXPECT_NE(x, y)                                                             \
    if ((x) == (y))                                                                    \
    {                                                                                  \
        UBENCH_PRINTF_INNER("Test fail (\"" #x " != " #y "\"): %lld vs %lld\n", x, y); \
        UB_ON_FAIL;                                                                    \
    }

#endif