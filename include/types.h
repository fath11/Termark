#ifndef MARK_H
#define MARK_H

#include <linux/limits.h>

struct Mark {
  char name[PATH_MAX];
  char path[PATH_MAX];
};

#endif
