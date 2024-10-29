#pragma once
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>


int setup_env();
int build_package(const char* target);

