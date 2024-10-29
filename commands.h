#pragma once
#include "installation/install.h"
#include "installation/buildenv.h"

#define ADD_COMMAND(_cmd_short, _cmd_long, _is_auth, _function) { \
        .cmd_short=_cmd_short,   \
        .cmd_long=_cmd_long,     \
        .is_auth=_is_auth,       \
        .function=_function      \
},

struct valid_commands {

        const char cmd_short; //short (one letter) version of a command
        const char* cmd_long; //long (--*) commands
        bool is_auth;
        int (* function)(int argc, void** argv);

} const commands[8] ={};