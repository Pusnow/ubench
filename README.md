# ubench

[![Linux Userspace Test](https://github.com/Pusnow/ubench/actions/workflows/linux-test.yml/badge.svg)](https://github.com/Pusnow/ubench/actions/workflows/linux-test.yml)
[![Kernel Mode Test](https://github.com/Pusnow/ubench/actions/workflows/kernel-test.yml/badge.svg)](https://github.com/Pusnow/ubench/actions/workflows/kernel-test.yml)
[![macOS Userspace Test](https://github.com/Pusnow/ubench/actions/workflows/macos-test.yml/badge.svg)](https://github.com/Pusnow/ubench/actions/workflows/macos-test.yml)
[![Windows Userspace Test](https://github.com/Pusnow/ubench/actions/workflows/windows-test.yml/badge.svg)](https://github.com/Pusnow/ubench/actions/workflows/windows-test.yml)

A header-only simple microbenchmark library for Linux user-space, Linux kernel module, macOS, and Windows.

## Quickstart

```c
#include "ubench.h" // --- 1. Download and include `ubench.h` file.

int main(){
  ubench_stat_t st; // ---- 2. Declare `ubench` struct. In this case `ubench_stat_t`.
  ubench_stat_init(&st); // ---- 3. Init `ubench` struct

  for (int i = 0; i < 1000; ++i) { // ---- 4. Repeat 1,000 times
    ubench_uint_t before = ubench_get_cycles(); // ---- 5. Retrive current cycles
    do_work(); // ---- 6. Do work to measure
    ubench_uint_t after = ubench_get_cycles(); // ---- 7. Retrive current cycles
    ubench_stat_add(&st, after - before); // ---- 8. Add difference to `st`
  }

  ubench_stat_print("measured", &st); // ---- 9. Print result;

  return 0;
}
```

### Output (Example)

Average of execution time in cycles.

```
[measured] avg: 107280 sum: 107280532 total: 1000
```

## API

### 1. Struct and Initialization

#### 1.1 Stat

Calculate average, summation, and count.

* Struct: `ubench_stat_t`
* Initialization

```c
void ubench_stat_init(ubench_stat_t* st);
```

#### 1.2 Variation

Calculate variation, average, summation, and count.

* Struct: `ubench_var_t`
* Initialization

```c
void ubench_var_init(ubench_stat_t* st);
```

#### 1.3 Histogram

Calculate log2 histogram.

* Struct: `ubench_hist_t`
* Initialization

```c
void ubench_hist_init(ubench_hist_t* st);
```

* Range
  * `0`
  * `[1-2)`
  * `[3-6)`
  * `[7-14)`
  * ...

#### 1.4 Higtogram Range

* Struct: `ubench_hist_range_t`
* Initialization

```c
void ubench_hist_range_init(ubench_hist_range_t* st, const ubench_uint_t min, const ubench_uint_t n, const ubench_uint_t d);
```

* `const ubench_uint_t min`: minimum value of range
* `const ubench_uint_t n`: numerator
* `const ubench_uint_t d`: denominator
* Range is determined by `min`, `n`, and `d`.
  * `[0 - min)`
  * `[min - min * n / d)`
  * `[min * n / d, min * n^2 / d^2)`
  * `[min * n^2 / d^2, min * n^3 / d^3)`
  * ...

### 2. Functions

#### 2.1 Get Cycles

Retrive cycles from CPU. For Intel CPUs, use `rdtsc` internally.

```c
ubench_uint_t ubench_get_cycles(void);
```

#### 2.2 Add Value

Push a value to calculate into struct.

```c
void ubench_stat_add(ubench_stat_t *st, const ubench_uint_t value);
void ubench_var_add(ubench_var_t *st, const ubench_uint_t value);
void ubench_hist_add(ubench_hist_t *st, const ubench_uint_t value);
void ubench_hist_range_add(ubench_hist_range_t *st, const ubench_uint_t value);
```

#### 2.3 Print Result

Calculate and print result from struct. Use `msg` to add a message in front of the result.

```c
void ubench_stat_print(const char* msg, ubench_stat_t *st);
void ubench_var_print(const char* msg, ubench_var_t *st);
void ubench_hist_print(const char* msg, ubench_hist_t *st);
void ubench_hist_range_print(const char* msg, ubench_hist_range_t *st);
```

#### 2.4 Dump Result

Calculate and print *easy-to-copy-and-paste* result from struct. Use `msg` to add a message in front of the result.

```c
void ubench_stat_dump(const char* msg, ubench_stat_t *st);
void ubench_var_dump(const char* msg, ubench_var_t *st);
void ubench_hist_dump(const char* msg, ubench_hist_t *st);
void ubench_hist_range_dump(const char* msg, ubench_hist_range_t *st);
```

#### 2.5 Reset Struct

Discared previously added values.

```c
void ubench_stat_reset(ubench_stat_t *st);
void ubench_var_reset(ubench_var_t *st);
void ubench_hist_reset(ubench_hist_t *st);
void ubench_hist_range_reset(ubench_hist_range_t *st);
```

#### 2.6 Add Value and Print/Dump Result

Push a value to calculate into struct, and print/dump result periodically.

* `freq`: every `freq` additions, print/dump result.

```c
void ubench_stat_add_print(const char *msg, const ubench_uint_t freq, ubench_stat_t *st, const ubench_uint_t value);
void ubench_var_add_print(const char *msg, const ubench_uint_t freq, ubench_var_t *st, const ubench_uint_t value);
void ubench_hist_add_print(const char *msg, const ubench_uint_t freq, ubench_hist_t *st, const ubench_uint_t value);
void ubench_hist_range_add_print(const char *msg, const ubench_uint_t freq, ubench_hist_range_t *st, const ubench_uint_t value);
void ubench_stat_add_dump(const char *msg, const ubench_uint_t freq, ubench_stat_t *st, const ubench_uint_t value);
void ubench_var_add_dump(const char *msg, const ubench_uint_t freq, ubench_var_t *st, const ubench_uint_t value);
void ubench_hist_add_dump(const char *msg, const ubench_uint_t freq, ubench_hist_t *st, const ubench_uint_t value);
void ubench_hist_range_add_dump(const char *msg, const ubench_uint_t freq, ubench_hist_range_t *st, const ubench_uint_t value);
```

#### 2.7 Add Value, Print/Dump Result, and Reset

Push a value to calculate into struct, print/dump result periodically, and reset struct.

```c
void ubench_stat_add_print_reset(const char *msg, const ubench_uint_t freq, ubench_stat_t *st, const ubench_uint_t value);
void ubench_var_add_print_reset(const char *msg, const ubench_uint_t freq, ubench_var_t *st, const ubench_uint_t value);
void ubench_hist_add_print_reset(const char *msg, const ubench_uint_t freq, ubench_hist_t *st, const ubench_uint_t value);
void ubench_hist_range_add_print_reset(const char *msg, const ubench_uint_t freq, ubench_hist_range_t *st, const ubench_uint_t value);
void ubench_stat_add_dump_reset(const char *msg, const ubench_uint_t freq, ubench_stat_t *st, const ubench_uint_t value);
void ubench_var_add_dump_reset(const char *msg, const ubench_uint_t freq, ubench_var_t *st, const ubench_uint_t value);
void ubench_hist_add_dump_reset(const char *msg, const ubench_uint_t freq, ubench_hist_t *st, const ubench_uint_t value);
void ubench_hist_range_add_dump_reset(const char *msg, const ubench_uint_t freq, ubench_hist_range_t *st, const ubench_uint_t value);
```

### 3. Configuration

Define macros to configure `ubench`.

Example:

```c
#define UBENCH_HIST_PRINT_ALL
#include "ubench.h"
```

#### 3.1 `UBENCH_HIST_PRINT_ALL`

By default, `hist` and `hist_range` do not print slots without values.
`UBENCH_HIST_PRINT_ALL` enables printing all slots in the structs.

#### 3.2 `UBENCH_HIST_PRINT_CUM`

Use cumulative result for `hist` and `hist_range`.

#### 3.3 `UBENCH_PREFIX` (*Not Implemented*)

Change prefix of structs and functions from `ubench` to `UBENCH_PREFIX`.

#### 3.4 `UBENCH_PRINTF(...)`

Change printf function to print and dump.
