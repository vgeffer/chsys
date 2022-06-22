#pragma once
#include "installation/install.h"
#include "installation/buildenv.h"

#define ADD_COMMAND(CSHORT, CLONG, AUTH, CALLBACK) { CSHORT, CLONG, AUTH, CALLBACK },

const char* parent_commands[] = {
        "environment",
        "package",
        "repos"
};

struct valid_commands {

    const char com_short; //short (one letter) version of a command
    const char* com_clong; //long (--*) commands

    int (* auth_fun)(); //assign, if sudo auth is required
    int (* callback_fun)(int argc, void** argv);

} commands[][8] = {
        { //environment
                ADD_COMMAND('n', "--new",     NULL, NULL)
                ADD_COMMAND('d', "--delete",  NULL, NULL)
                ADD_COMMAND('e', "--enter",   NULL, NULL)
                ADD_COMMAND('r', "--repair",  NULL, NULL)
        },
        { //package
                ADD_COMMAND('i', "--install",   NULL, NULL)
                ADD_COMMAND('r', "--remove",    NULL, NULL)
                ADD_COMMAND('e', "--env",       NULL, NULL)
                ADD_COMMAND('x', "--xcl",       NULL, NULL)
                ADD_COMMAND('d', "--download",  NULL, NULL)
                ADD_COMMAND('n', "--notree",    NULL, NULL)
                ADD_COMMAND('q', "--query",     NULL, NULL)
                ADD_COMMAND('l', "--local",     NULL, NULL)
        },
        { //repos
                ADD_COMMAND('l', "--list",       NULL, NULL)
                ADD_COMMAND('r', "--rate",       NULL, NULL)
                ADD_COMMAND('a', "--add",        NULL, NULL)
                ADD_COMMAND('R', "--remove",     NULL, NULL)
                ADD_COMMAND('u', "--update",     NULL, NULL)
                ADD_COMMAND('A', "--activate",   NULL, NULL)
                ADD_COMMAND('D', "--deactivate", NULL, NULL)
        },
};
