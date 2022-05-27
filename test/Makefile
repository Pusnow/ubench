# kmod for test

ifdef TEST_NAME

KERNEL_DIR ?= /lib/modules/`uname -r`/build

default:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) EXTRA_CFLAGS=-DUBENCH_TEST_NAME=$(TEST_NAME) modules

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

obj-m := ${TEST_NAME}.o

kmod_${TEST_NAME}-objs = ${TEST_NAME}.o


else

SRCS = $(filter-out $(wildcard *.mod.c),$(wildcard *.c))
KMODS = $(SRCS:.c=.ko)


.PHONY: all
all: $(KMODS)

out:
	@mkdir -p out
	@cp ../ubench.h out


%.ko: out %.c
	@mkdir -p out/$*
	@cp Makefile out/$*
	@cp $*.c out/$*
	@cp common.h out/$*
	@cd out/$* && $(MAKE) TEST_NAME=$*

clean:
	rm -r out

endif