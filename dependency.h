

#pragma once
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/file.h>
#include <sys/stat.h>


typedef enum {
    DEPTYPE_BUILD               = 0 << 0,
    DEPTYPE_RUNTIME             = 1 << 0,

    DEPTYPE_OPTIONAL            = 0 << 1,
    DEPTYPE_REQUIRED            = 1 << 1,

    //On some systems, deps like make may not be tracked by chsys
    DEPFLAG_CHECK_PKG           = 0 << 2,
    DEPFLAG_CHECK_FILE          = 1 << 2

}pkg_depflags;


typedef struct {

    uint8_t pkg_flags;
    char* pkg_name;

}pkg_dependency;

int check_install_deps();
int check_dep(pkg_dependency* dep);