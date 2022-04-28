SHELL := /bin/bash
CCFLAGS := -O2 -std=c99 -march=native -m64 -g -Wall
LOGFILE = .compilelog

src := $(shell find -name \*.c)
obj := $(patsubst %.c, build/%.o, $(src))

$(obj): build/%.o : %.c
	@echo building target: $@ &&\
	mkdir -p $(dir $@) && \
	gcc $(CCFLAGS) -c $(patsubst build/%.o, %.c, $@) -o $@ 2> $(LOGFILE)

.PHONY: build, install, clean
build: $(obj)
	@echo linking target: chsys && \
	gcc build/chsys-install.o build/util.o build/installation/install.o build/package/dependency.o -o chsys


install:

clean:
	rm -r build
