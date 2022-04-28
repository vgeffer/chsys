//
// chsys: install
// (c) giantdwarf 2022
// module containing code for installation of binary packages
//

#pragma once
#define _BSD_SOURCE

#include <dirent.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include <pwd.h>
#include <grp.h>

#include "../package/dependency.h"
#include "../util.h"


int install(char* pk_path);

//DONE!
int copy_out_files(char* pk_origin, char* pk_target, bool silent);

int worker_build (int uid, const char* pk_path, const char* pk_home);
int worker_install(int uid, const char* pk_path, const char* pk_home);

int build_handler(const char* pk_name, const char* pk_path);
int install_handler (const char* pk_name, const char* pk_path);

int rem(const char* pk_name);
int remove_recursive(char* pk_origin, char* pk_target, bool root);
int query();