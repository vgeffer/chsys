//
// chsys: install
// (c) giantdwarf 2022
// module containing code for reading and parsing package metadata
//
#pragma once


typedef struct {

}pkg_metadata;


int parse_metadata(char* buffer, size_t len, pkg_metadata* metadata);