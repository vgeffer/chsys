#pragma once
#include "../package/package.h"

struct chsys_repo {


    int package_count;
    struct chsys_package* packages;

};


int load_repo(const char* filename, struct chsys_repo* repo);
chsys_package* find_package(struct chsys_repo* repo, const char* uuid);
