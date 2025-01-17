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
