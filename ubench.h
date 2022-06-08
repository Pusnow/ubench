#ifndef __UBENCH_UBENCH_H
#define __UBENCH_UBENCH_H

/* declaration */

#if defined(__GNUC__) || defined(__MINGW__) || defined(__clang__)
#define UBENCH_GCC_LIKE

#define UBENCH_STATIC_ALWAYS_INLINE_INNER static inline __attribute__((always_inline))

#define UBENCH_LIKELY(x) __builtin_expect(!!(x), 1)
#define UBENCH_UNLIKELY(x) __builtin_expect(!!(x), 0)

typedef unsigned long long ubench_uint_t;

#elif defined(_MSC_VER)
#define UBENCH_MSC_LIKE

#define UBENCH_STATIC_ALWAYS_INLINE_INNER static inline __forceinline

#define UBENCH_LIKELY(x) (x)
#define UBENCH_UNLIKELY(x) (x)

#if defined(__x86_64__)
#include <intrin.h>
#pragma intrinsic(__rdtsc)
#endif

typedef unsigned long long ubench_uint_t;
#endif

#ifndef UBENCH_SYMBOL_MODE
#define UBENCH_STATIC_ALWAYS_INLINE UBENCH_STATIC_ALWAYS_INLINE_INNER
#define UBENCH_STATIC_INLINE static inline

#else

// Source: https://gcc.gnu.org/wiki/Visibility
#if defined _WIN32 || defined __CYGWIN__
#ifdef UBENCH_BUILDING_DLL
#ifdef __GNUC__
#define UBENCH_DLL_PUBLIC __attribute__((dllexport))
#else
#define UBENCH_DLL_PUBLIC __declspec(dllexport) // Note: actually gcc seems to also supports this syntax.
#endif
#else
#ifdef __GNUC__
#define UBENCH_DLL_PUBLIC __attribute__((dllimport))
#else
#define UBENCH_DLL_PUBLIC __declspec(dllimport) // Note: actually gcc seems to also supports this syntax.
#endif
#endif
#define UBENCH_DLL_LOCAL
#else
#if __GNUC__ >= 4
#define UBENCH_DLL_PUBLIC __attribute__((visibility("default")))
#define UBENCH_DLL_LOCAL __attribute__((visibility("hidden")))
#else
#define UBENCH_DLL_PUBLIC
#define UBENCH_DLL_LOCAL
#endif
#endif

#define UBENCH_STATIC_ALWAYS_INLINE UBENCH_DLL_PUBLIC
#define UBENCH_STATIC_INLINE UBENCH_DLL_PUBLIC
#endif

#define UBENCH_DECLARE_INIT(func_name, ...) UBENCH_STATIC_ALWAYS_INLINE void ubench_##func_name##_init(ubench_##func_name##_t *s, ##__VA_ARGS__)
#define UBENCH_DECLARE_RESET(func_name) UBENCH_STATIC_ALWAYS_INLINE void ubench_##func_name##_reset(ubench_##func_name##_t *s)
#define UBENCH_DECLARE_ADD(func_name) UBENCH_STATIC_ALWAYS_INLINE void ubench_##func_name##_add(ubench_##func_name##_t *s, const ubench_uint_t value)
#define UBENCH_DECLARE_PRINT(func_name, print_name) UBENCH_STATIC_ALWAYS_INLINE void ubench_##func_name##_##print_name(const char *msg, ubench_##func_name##_t *s)
#define UBENCH_DECLARE_ADD_PRINT(func_name, print_name) UBENCH_STATIC_ALWAYS_INLINE void ubench_##func_name##_add_##print_name(const char *msg, const ubench_uint_t freq, ubench_##func_name##_t *s, const ubench_uint_t value)
#define UBENCH_DECLARE_ADD_PRINT_RESET(func_name, print_name) UBENCH_STATIC_ALWAYS_INLINE void ubench_##func_name##_add_##print_name##_reset(const char *msg, const ubench_uint_t freq, ubench_##func_name##_t *s, const ubench_uint_t value)

struct ubench_stat_s
{
    ubench_uint_t total;
    ubench_uint_t sum;
};

typedef struct ubench_stat_s ubench_stat_t;

struct ubench_var_s
{
    ubench_uint_t total;
    ubench_uint_t sum;
    ubench_uint_t sum_squares;
};

typedef struct ubench_var_s ubench_var_t;

#define UBENCH_HIST_LEVEL (sizeof(ubench_uint_t) << 3)

struct ubench_hist_s
{
    ubench_uint_t total;
    ubench_uint_t counts[UBENCH_HIST_LEVEL];
};
typedef struct ubench_hist_s ubench_hist_t;

struct ubench_hist_range_s
{
    ubench_uint_t total;
    ubench_uint_t keys[UBENCH_HIST_LEVEL - 1];
    ubench_uint_t counts[UBENCH_HIST_LEVEL];
};
typedef struct ubench_hist_range_s ubench_hist_range_t;

#define UBENCH_DECLARE_ALL(func_name, ...)            \
    UBENCH_DECLARE_INIT(func_name, ##__VA_ARGS__);    \
    UBENCH_DECLARE_RESET(func_name);                  \
    UBENCH_DECLARE_ADD(func_name);                    \
    UBENCH_DECLARE_PRINT(func_name, print);           \
    UBENCH_DECLARE_ADD_PRINT(func_name, print);       \
    UBENCH_DECLARE_ADD_PRINT_RESET(func_name, print); \
    UBENCH_DECLARE_PRINT(func_name, dump);            \
    UBENCH_DECLARE_ADD_PRINT(func_name, dump);        \
    UBENCH_DECLARE_ADD_PRINT_RESET(func_name, dump);

UBENCH_DECLARE_ALL(stat)
UBENCH_DECLARE_PRINT(stat, print_double);
UBENCH_DECLARE_ALL(var)
UBENCH_DECLARE_PRINT(var, print_double);
UBENCH_DECLARE_ALL(hist)
UBENCH_DECLARE_ALL(hist_range, const ubench_uint_t min, const ubench_uint_t n, const ubench_uint_t d)

/* declaration end */

/* definition */

#ifndef UBENCH_NO_DEFINITION

#if defined(__KERNEL__) && defined(UBENCH_GCC_LIKE)
#include <linux/printk.h>
#define UBENCH_PRINTF_INNER(...) pr_info(__VA_ARGS__)
#define UBENCH_PRINTF_PREFIX "ubench: "
#else
#include <stdio.h>
#define UBENCH_PRINTF_INNER(...) printf(__VA_ARGS__)
#define UBENCH_PRINTF_PREFIX ""
#endif

#ifndef UBENCH_PRINTF
#define UBENCH_PRINTF(...) UBENCH_PRINTF_INNER(UBENCH_PRINTF_PREFIX __VA_ARGS__)
#endif

UBENCH_STATIC_ALWAYS_INLINE ubench_uint_t ubench_get_cycles(void)
{
#if defined(UBENCH_GCC_LIKE) && defined(__x86_64__)
    return __builtin_ia32_rdtsc();
#elif defined(UBENCH_MSC_LIKE)
    return __rdtsc();
#else
#error "Unsupported platform"
#endif
}

#if !defined(UBENCH_GCC_LIKE) || !defined(__x86_64__)
static const int ubench_log2_table[64] = {
    0, 58, 1, 59, 47, 53, 2, 60, 39, 48, 27, 54, 33, 42, 3, 61,
    51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4, 62,
    57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21, 56,
    45, 25, 31, 35, 16, 9, 12, 44, 24, 15, 8, 23, 7, 6, 5, 63};

#endif

UBENCH_STATIC_ALWAYS_INLINE_INNER ubench_uint_t ubench_log2(ubench_uint_t x)
{
#if defined(UBENCH_GCC_LIKE)
    return (sizeof(unsigned long long) << 3) - __builtin_clzll(x) - 1;
#else
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    x |= x >> 32;

    return ubench_log2_table[(x * 0x03f6eaf2cd271461) >> 58];
#endif
}

#define UBENCH_DEFINE_ADD_PRINT(func_name, print_name) \
    UBENCH_DECLARE_ADD_PRINT(func_name, print_name)    \
    {                                                  \
        ubench_##func_name##_add(s, value);            \
        if (UBENCH_UNLIKELY(s->total % freq == 0))     \
        {                                              \
            ubench_##func_name##_##print_name(msg, s); \
        }                                              \
    }

#define UBENCH_DEFINE_ADD_PRINT_RESET(func_name, print_name) \
    UBENCH_DECLARE_ADD_PRINT_RESET(func_name, print_name)    \
    {                                                        \
        ubench_##func_name##_add(s, value);                  \
        if (UBENCH_UNLIKELY(s->total > freq))                \
        {                                                    \
            ubench_##func_name##_##print_name(msg, s);       \
            ubench_##func_name##_reset(s);                   \
        }                                                    \
    }

UBENCH_DECLARE_RESET(stat)
{
    s->total = 0;
    s->sum = 0;
}

UBENCH_DECLARE_INIT(stat)
{
    ubench_stat_reset(s);
}

UBENCH_DECLARE_ADD(stat)
{
    ++s->total;
    s->sum += value;
}

UBENCH_DECLARE_PRINT(stat, print)
{
    UBENCH_PRINTF("[%s] avg: %llu sum: %llu total: %llu\n", msg, s->sum / s->total, s->sum, s->total);
}

UBENCH_DECLARE_PRINT(stat, print_double)
{
    double avg = ((double)s->sum) / ((double)s->total);
    UBENCH_PRINTF("[%s] avg: %lf sum: %llu total: %llu\n", msg, avg, s->sum, s->total);
}

UBENCH_DECLARE_PRINT(stat, dump)
{
    ubench_stat_print(msg, s);
}

UBENCH_DEFINE_ADD_PRINT(stat, print)
UBENCH_DEFINE_ADD_PRINT_RESET(stat, print)
UBENCH_DEFINE_ADD_PRINT(stat, dump)
UBENCH_DEFINE_ADD_PRINT_RESET(stat, dump)

UBENCH_DECLARE_RESET(var)
{
    s->total = 0;
    s->sum = 0;
    s->sum_squares = 0;
}

UBENCH_DECLARE_INIT(var)
{
    ubench_var_reset(s);
}

UBENCH_DECLARE_ADD(var)
{
    ++s->total;
    s->sum += value;
    s->sum_squares += value * value;
}

UBENCH_DECLARE_PRINT(var, print)
{
    ubench_uint_t avg = s->sum / s->total;
    UBENCH_PRINTF("[%s] avg: %llu sum: %llu var: %llu total: %llu\n", msg, avg, s->sum, (s->sum_squares / s->total) - (avg * avg), s->total);
}
UBENCH_DECLARE_PRINT(var, print_double)
{
    double avg = ((double)s->sum) / ((double)s->total);
    double e_squres = ((double)s->sum_squares) / ((double)s->total);
    UBENCH_PRINTF("[%s] avg: %lf sum: %llu var: %lf total: %llu\n", msg, avg, s->sum, e_squres - avg * avg, s->total);
}

UBENCH_DECLARE_PRINT(var, dump)
{
    ubench_var_print(msg, s);
}

UBENCH_DEFINE_ADD_PRINT(var, print)
UBENCH_DEFINE_ADD_PRINT_RESET(var, print)
UBENCH_DEFINE_ADD_PRINT(var, dump)
UBENCH_DEFINE_ADD_PRINT_RESET(var, dump)

UBENCH_DECLARE_RESET(hist)
{
    int i = 0;
    s->total = 0;
    for (; i < UBENCH_HIST_LEVEL; ++i)
    {
        s->counts[i] = 0;
    }
}

UBENCH_DECLARE_INIT(hist)
{
    ubench_hist_reset(s);
}
UBENCH_DECLARE_ADD(hist)
{
    ubench_uint_t level;
    ++s->total;
    level = ubench_log2(value + 1);
    ++s->counts[level];
}

UBENCH_DECLARE_PRINT(hist, print)
{

    ubench_uint_t cum = 0;
    ubench_uint_t i = 0;
    for (; i < UBENCH_HIST_LEVEL - 1; ++i)
    {
#ifndef UBENCH_HIST_PRINT_ALL
        if (s->counts[i])
#endif
            UBENCH_PRINTF("[%s] [%llu - %llu): %llu\n", msg, ((ubench_uint_t)1 << i) - 1, ((ubench_uint_t)1 << (i + 1)) - 1, cum + s->counts[i]);
#ifdef UBENCH_HIST_PRINT_CUM
        cum += s->counts[i];
#endif
    }

#ifndef UBENCH_HIST_PRINT_ALL
    if (s->counts[UBENCH_HIST_LEVEL - 1])
#endif
        UBENCH_PRINTF("[%s] [%llu - ): %llu\n", msg, ((ubench_uint_t)1 << i) - 1, cum + s->counts[i]);
}

UBENCH_DECLARE_PRINT(hist, dump)
{
    ubench_uint_t i = 1;
    UBENCH_PRINTF("[%s] count: %lld", msg, s->counts[0]);
    for (; i < UBENCH_HIST_LEVEL; ++i)
    {
        UBENCH_PRINTF(",%llu", s->counts[i]);
    }
    UBENCH_PRINTF("\n");
}

UBENCH_DEFINE_ADD_PRINT(hist, print)
UBENCH_DEFINE_ADD_PRINT_RESET(hist, print)
UBENCH_DEFINE_ADD_PRINT(hist, dump)
UBENCH_DEFINE_ADD_PRINT_RESET(hist, dump)

UBENCH_DECLARE_RESET(hist_range)
{
    int i = 0;
    s->total = 0;
    for (; i < UBENCH_HIST_LEVEL; ++i)
    {
        s->counts[i] = 0;
    }
}

UBENCH_DECLARE_INIT(hist_range, const ubench_uint_t min, const ubench_uint_t n, const ubench_uint_t d)
{
    int i = 1;
    s->keys[0] = min;
    for (; i < UBENCH_HIST_LEVEL - 1; ++i)
    {
        s->keys[i] = (s->keys[i - 1] * n) / d;
    }

    ubench_hist_range_reset(s);
}

UBENCH_DECLARE_ADD(hist_range)
{
    int i;
    int start = -1;
    ++s->total;
    for (i = 5; i >= 0; --i)
    {
        start += (value >= s->keys[start + (1 << i)]) << i;
    }
    ++s->counts[start + 1];
}

UBENCH_DECLARE_PRINT(hist_range, print)
{
    ubench_uint_t i;
#ifdef UBENCH_HIST_PRINT_CUM
    ubench_uint_t cum = s->counts[0];
#else
    ubench_uint_t cum = 0;
#endif
#ifndef UBENCH_HIST_PRINT_ALL
    if (s->counts[0])
#endif
        UBENCH_PRINTF("[%s] [0 - %llu): %llu\n", msg, s->keys[0], s->counts[0]);

    for (i = 1; i < UBENCH_HIST_LEVEL - 1; ++i)
    {
#ifndef UBENCH_HIST_PRINT_ALL
        if (s->counts[i])
#endif
            UBENCH_PRINTF("[%s] [%llu - %llu): %llu\n", msg, s->keys[i - 1], s->keys[i], cum + s->counts[i]);
#ifdef UBENCH_HIST_PRINT_CUM
        cum += s->counts[i];
#endif
    }

#ifndef UBENCH_HIST_PRINT_ALL
    if (s->counts[UBENCH_HIST_LEVEL - 1])
#endif
        UBENCH_PRINTF("[%s] [%llu - ): %llu\n", msg, s->keys[UBENCH_HIST_LEVEL - 2], cum + s->counts[UBENCH_HIST_LEVEL - 1]);
}

UBENCH_DECLARE_PRINT(hist_range, dump)
{
    ubench_uint_t i;
    UBENCH_PRINTF("[%s] key: 0", msg);
    for (i = 0; i < UBENCH_HIST_LEVEL - 1; ++i)
    {
        UBENCH_PRINTF(",%lld", s->keys[i]);
    }
    UBENCH_PRINTF("\n[%s] count: %lld", msg, s->counts[0]);
    for (i = 1; i < UBENCH_HIST_LEVEL; ++i)
    {
        UBENCH_PRINTF(",%lld", s->counts[i]);
    }
    UBENCH_PRINTF("\n");
}
UBENCH_DEFINE_ADD_PRINT(hist_range, print)
UBENCH_DEFINE_ADD_PRINT_RESET(hist_range, print)
UBENCH_DEFINE_ADD_PRINT(hist_range, dump)
UBENCH_DEFINE_ADD_PRINT_RESET(hist_range, dump)

/* definition end */
#endif
#endif