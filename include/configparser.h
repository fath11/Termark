#ifndef CONFIGPARSER_H
#define CONFIGPARSER_H

#include <stdio.h>

#include "types.h"

void parseConfig(FILE *stream, struct Mark *config[], int *config_len);
void freeConfig(struct Mark *config[], int count);

#endif
