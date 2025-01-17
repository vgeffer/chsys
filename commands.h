/////////////////////////////////////////////////////////////////////////////////
//
//    chsys - experimental binary/source package manager
//    Copyright (C) 2022-2025 geffevil <geffevil@fel.cvut.cz>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
/////////////////////////////////////////////////////////////////////////////////
#pragma once

#define ADD_COMMAND(_cmd_short, _cmd_long, _is_auth, _function) { \
        .cmd_short=_cmd_short,   \
        .cmd_long=_cmd_long,     \
        .is_auth=_is_auth,       \
        .function=_function      \
},

struct valid_commands {

        const char cmd_short; //short (one letter) version of a command
        const char* cmd_long; //long (--*) commands
        char is_auth;
        int (* function)(int argc, void** argv);

} const commands[8] = {};