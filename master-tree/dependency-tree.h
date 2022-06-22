#pragma once
#include <stdio.h>
#include <unistd.h>

#include "tree_metadata.h"
#include "../util.h"

typedef struct {

} deptree_node;

deptree_node* get_node(const char nodeUUID[8]);