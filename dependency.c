#include "dependency.h"

int check_install_deps() {

    const char* install_deps[] = {
        "/usr/bin/tar",
        "/usr/sbin/useradd"
    };

    printf("Checking installation dependencies:\n");
    struct stat depcheck;

    //Check for build dependecies
    for (int i = 0;  i < sizeof(install_deps) / sizeof(install_deps[0]); i++) {

        printf("Checking for installation dependency: %s...", install_deps[i]);
        if (stat(install_deps[i], &depcheck) != 0) {
            printf("\e[1;31mNOT FOUND\e[m\n");
            return 0;
        }
        
        printf("OK\n");
    }
    printf("\n");
    return 1;
}


int check_dep(pkg_dependency* dep) {
    
    struct stat depcheck;
    for (int i = 0;  i < sizeof(dep) / sizeof(dep); i++) {

        printf("Checking for build dependency: %s...", dep->pkg_name);


        if (stat(dep->pkg_name, &depcheck) != 0) {
            printf("\e[1;31mNOT FOUND\e[m\n");
            
            if (dep->pkg_flags & DEPTYPE_REQUIRED) {
                printf("\e[0;31m\nRequired build dependency %s not found! Exiting...\e[m\n", dep->pkg_name);
                exit(-1);
            }
        }
        else printf("OK\n");
    }
}