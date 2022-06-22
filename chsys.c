    #include <stdbool.h>
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #include "commands.h"
    #include "util.h"

    Mutex* install_lock = NULL;

    //For handling Ctrl-C and Ctrl-D signals
    int signal_handler() {

    }

    void help () {

        printf("chsys package manager\n");
        printf("(c) made by giantdwarf in 2022\n\n");

        printf("Usage:\n");
        printf(" chsys-install [action] [package path]\n\n");

        printf("Help: help\n");
        printf(" display this screen\n\n");

        printf("Environments: environment [switches]\n");
        printf(" n / --new: create new environment inv /var/env\n");
        printf(" d / --delete: delete environment\n");
        printf(" e / --enter: enter virtual environment\n");
        printf(" r / --repair: scan and repair any broken environments\n\n");

        printf("Packages: package [switches]\n");
        printf(" i / --install: install package to the designated environment (default: all)\n");
        printf(" r / --remove: remove installed package (if not found in all or specified otherwise, search though all environments)\n");
        printf(" e / --env: install package only in specified environment\n");
        printf(" x / --xcl: install package in all environments except selected one\n");
        printf(" d / --download: do not install package, download only\n");
        printf(" n / --notree: do not add package to master dependency tree\n");
        printf(" q / --query: query all installed packages\n");
        printf(" l / --local: use local folder as package repository\n\n");

        printf("Repositories: repos\n");
        printf(" l / --list: list all repositories\n");
        printf(" r / --rate: rate all repositories based on their download speeds\n");
        printf(" a / --add: add repository to repository list\n");
        printf(" R / --remove: remove repository from repository list (denoted by id)\n");
        printf(" u / --update: update local dependency trees of all local repositories\n");
        printf(" A / --activate: enable previously disabled repository (denoted by id)\n");
        printf(" D / --deactivate: temporarily disable repository (denoted by id)\n");

        exit(0);
    }
    void version () {


        exit(0);
    }


    int main (int argc, char* argv[]) {

        //There always needs to be at least one arg (or explicitly check for help)
        if (argc < 2 || strcmp(argv[1], "help") == 0)
            help();


        if (strcmp(argv[1], "version") == 0) {
            version();
        }

        //Itterate through parent args
        for (size_t parg = 0; parg < ARRAY_LENGTH(parent_commands); parg++) {
            if (strcmp(argv[1], parent_commands[parg]) != 0) continue; //If not current command, skip

            if (argc < 3) {
                //TODO: figure out what i want to do in here
                return -1;
            }

            //Go through the rest of argv
            for (size_t args = 3; args < argc; args++) {

                //Check if arg starts with --
                if (strncmp(argv[args], "--", 2)) {
                    //arg starts with --, fid out which

                    for (int com = 0; com < ARRAY_LENGTH(commands[parg]); com++) {
                        if (strcmp(commands[parg][com].com_short, argv[args]) == 0) {

                            if (commands[parg][com].auth_fun && !commands[parg][com].auth_fun()) {
                                printf("You need root permissions to change your system! Run chsys with sudo, su or doas\n");
                                return -1;
                            }

                            //If you're here, you have root or no root is required

                            //Return value explanation:
                            // if < 0: error code
                            // if >= 0: number of args used by function
                            int result = commands[parg][com].callback_fun(argc - args, &(argv[args]));

                            if (result < 0) {
                                printf("Fatal error occurred! Exiting...\n");
                            }
                        }

                    }
                }
                //Otherwise, treat as letter args
                else {
                    //For safety, create backup of action string
                    char* com_str = (char*)malloc(strlen(argv[args]) + 1); //Add null byte
                    memset(com_str, '\00', strlen(argv[args] + 1));
                    strcpy(com_str, argv[args]);


                    //Parse out letter actions
                    for (size_t com_char = 0; com_str[com_char]; com_char++) {

                        //Get the action for corresponding letter
                        for (int com = 0; com < ARRAY_LENGTH(commands[parg]); com++) {

                            if (com_str[com_char] == commands[parg][com].com_short) {
                                if (commands[parg][com].auth_fun && !commands[parg][com].auth_fun()) {
                                    printf("You need root permissions to change your system! Run chsys with sudo, su or doas\n");
                                    return -1;
                                }

                                //If you're here, you have root or no root is required

                                //Return value explanation:
                                // if < 0: error code
                                // if >= 0: number of args used by function
                                int result = commands[parg][com].callback_fun(argc - args, &(argv[args]));

                                if (result < 0) {
                                    printf("Fatal error occurred! Exiting...\n");
                                }
                            }
                        }
                    }

                    free(commands);
                }
            }




        }
        /*install_lock = create_mutex("/var/lock/chsys");
        if (install_lock == NULL) {
            printf("Unable to create lockfile. Please create file /var/chsys/lock manually.\n");
            return -1;
        }

        if (try_lock(install_lock) != 0) {
            printf("Unable to secure exclusive access to resources. Is another instance running?\n");
            return -1;
        }

        else*/

        printf("Unknow action: %s\n", argv[1]);


        unlock(install_lock);
        destroy_mutex(install_lock);
    }
