#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "installation/install.h"
#include "util.h"

Mutex* install_lock = NULL;

//For handling Ctrl-C and Ctrl-D signals
int signal_handler() {

}

void help () {

    printf("chsys package manager\n");
    printf("(c) giantdawrf 2022\n\n");

    printf("Usage:\n");
    printf(" chsys-install [switches] [action] [package path]\n\n");

    printf(" d = download package from repositories\n");
    printf(" f = use local package archive\n");
    printf(" o = override, force action (not recomended using)\n");
    printf(" r = recursive (also removes package dependecies)\n");

    printf("Special packages:\n");
    printf(" world: repository definition for main repository of chsys. \n");
    printf(" core: repository definition for base repository containing linux and other base programs.\n");
    printf(" all: every installed package on the system.\n\n");

    printf("Example usage: \n");
    printf(" Updating all packages: chsys-install U world all\n");
    printf(" Install package from repository: chsys-install Ad package\n");
    printf(" Uninstall package with all of it's dependencies: chsys-install Rr package\n");

    exit(0);
}



int main (int argc, char* argv[]) {

    //There always needs to be at least one arg
    if (argc < 2)
        help();

    //Check if user is root
    if (geteuid() != 0) {
        fprintf(stderr, "To modify your system, chsys needs to run as root.\n");
        return 1;
    }

    install_lock = create_mutex("/var/chsys/lock");
    if (install_lock == NULL) {
        printf("Unable to create lockfile. Please create file /var/chsys/lock manually.\n");
        return -1;
    }

    if (try_lock(install_lock) != 0) {
        printf("Unable to secure exclusive access to resources. Is another instance running?\n");
        return -1;
    }
    
    if (strcmp(argv[1], "add") == 0)
        install(argv[2]);

    else if (strcmp(argv[1], "rem") == 0) {
        rem(argv[2]);
    }

    else
        printf("Unknow action: %s\n", argv[1]);


    unlock(install_lock);
    destroy_mutex(install_lock);
}
