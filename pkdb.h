#pragma once

#include <stdio.h>
#include <sys/stat.h>

static int pkadd(char* name, char* version) {

    FILE* pkdb;
    struct stat db;
    if (stat("/etc/chsysdb", &db) != 0) {
        pkdb = fopen("/etc/chsysdb", "w");
    }   
    else pkdb = fopen("/etc/chsysdb", "a");

    fprintf(pkdb, "%s: v. %s\n", name, version);
    fclose(pkdb);
}

