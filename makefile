SHELL := /bin/bash
CCFLAGS := -O2 -std=c99 -march=native -m64 -g -Wall
LOGFILE = .compilelog

src := $(shell find *.c)
obj := $(patsubst %.c, build/%.o, $(src))

$(obj): build/%.o : %.c
	mkdir -p $(dir $@)
	gcc $(CCFLAGS) -c $(patsubst build/%.o, %.c, $@) -o $@ 2> $(LOGFILE)

.PHONY: build, install, clean
build: $(obj)
	gcc build/chsys-install.o build/util.o build/install.o build/dependency.o -o chsys-install
	gcc build/chsys-query.o -o chsys-query


install:

clean:
	rm -r build
