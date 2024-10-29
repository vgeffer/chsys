#pragma once
#include <stdbool.h>

struct chsys_package {

    const char pkg_uuid[16]; // 128-bit 

    const char name[64];
    const char author[64];
    const char email[64];

    const char version[16];

    int deps_pkg_uuids_count;
    const char* deps_pkg_uuids[16];

    int pkg_conflict_uuids_count;
    const char* pkg_conflict_uuids[16];

    /* TODO: cryptography*/

    bool source;
    

} __attribute__((packed));