#include "repo.h"
#include <unistd.h>
#include <sys/types.h>

static struct chsys_package* binsearch(struct chsys_repo* repo,  const char[16] uuid);