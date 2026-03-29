#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/configparser.h"
#include "../include/const.h"
#include "../include/types.h"

/*
 * The config file format is as follow:
 *
 * name, path,
 * name, path, ...
 *
 * curr_t_type == 0 refers to name
 * curr_t_type == 1 refers to path
 *
 * */
void parseConfig(FILE *stream, struct Mark *config[], int *config_len) {

  int config_i = 0;

  struct Mark *token = malloc(sizeof(struct Mark));

  int t_i = 0;
  int curr_t_type = 0;

  char curr_char;
  while ((curr_char = fgetc(stream)) != EOF && config_i < MAX_CONFIG_SIZE) {
    if (curr_char == '\n' || curr_char == ' ')
      continue;

    if (curr_char == ',') {
      if (curr_t_type == 0) {
        token->name[t_i] = '\0';
      } else {
        token->path[t_i] = '\0';
        // We have parsed one complete entry, copy it
        config[config_i] = malloc(sizeof(struct Mark));
        strcpy(config[config_i]->name, token->name);
        strcpy(config[config_i]->path, token->path);

        config_i++;
      }

      curr_t_type = !curr_t_type;
      t_i = 0;
      continue;
    }

    if (curr_t_type == 0) {
      token->name[t_i] = curr_char;
    } else {
      token->path[t_i] = curr_char;
    }
    t_i++;
  }

  *config_len = config_i;

  free(token);
}

void freeConfig(struct Mark *config[], int count) {
  for (int i = 0; i < count; i++) {
    if (config[i] != NULL) {
      free(config[i]);
      config[i] = NULL;
    }
  }
}
