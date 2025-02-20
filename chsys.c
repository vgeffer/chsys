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
#include <signal.h>
#include <stdlib.h>


/* For handling Ctrl-C and Ctrl-D signals */
void signal_handler(int sig) {

}

void help () {;
    exit(0);
}

void version () {
    exit(0);
}

int main (int argc, char* argv[]) {
    //There always needs to be at least one arg (or explicitly check for help)
    if (argc < 2)
        help();


    /* Setup signal handlers */
    if (signal(SIGTERM, &signal_handler) == SIG_ERR) {
        exit(-1);
    }
}
