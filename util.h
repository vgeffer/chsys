//
// chsys: util
// (c) giantdwarf 2022
// module containing utility functions for chsys package manager
//


#pragma once

#define _XOPEN_SOURCE 600 
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

//Concat two strings into one
char* concat(char* a, char* b);

//Split one string into many, based on given delimeter
char** split(char* a, char delim);

int copy_file(char* src, char* dest);

//=============================
// IMPORTANT WARNING!
// This mutex implementation is
// NOT deadlock secure!
//
// USE AT OWN RISK!
//=============================
typedef struct {
    int lock;
} Mutex;

Mutex* create_mutex(char* lock_file);
int destroy_mutex(Mutex* m);

//Lock. If locked, wait till unlocked
int lock(Mutex* m);

//Try locking. If locked, exit imidiately
int try_lock(Mutex* m);

//Unlock mutex
int unlock(Mutex* m);