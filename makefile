.DEFAULT_GOAL := chsys
SHELL := /bin/bash
CCFLAGS := -std=c99 -march=native -m64 -g -Wall -D_DEFAULT_SOURCE

src := $(shell find -name \*.c)
obj := $(patsubst %.c, build/%.o, $(src))

$(obj): build/%.o : %.c
	@echo building target: $@ &&\
	mkdir -p $(dir $@) && \
	gcc $(CCFLAGS) -c $(patsubst build/%.o, %.c, $@) -o $@ 

chsys: $(obj)
	@echo linking target: chsys && \
	gcc $(obj) -o chsys


.PHONY: install, clean

install:

clean:
	rm -rf build
	rm -f chsys
