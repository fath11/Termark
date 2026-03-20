#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct Mark {
  char name[PATH_MAX];
  char path[PATH_MAX];
};

void parseConfig(FILE *stream, struct Mark *config[]);
void freeConfig(struct Mark *config[], int count);

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "Please provide an argument\n");
    exit(EXIT_FAILURE);
  }

  int opt;
  while ((opt = getopt(argc, argv, "l")) != -1) {
    switch (opt) {
    case 'l':
      printf("Mark: Path\n");
      exit(EXIT_SUCCESS);
    case '?':
      exit(EXIT_FAILURE);
    }
  }

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof cwd) != NULL) {
    printf("Current working directory: %s\n", cwd);
  } else {
    perror("getcwd() error");
    return 1;
  }

  char markname[PATH_MAX];
  if (sizeof(argv[1]) <= sizeof markname) {
    strcpy(markname, argv[1]);
    printf("%s\n", markname);
  } else {
    fprintf(stderr, "Markname is too long, maximum %i bytes\n", PATH_MAX);
  }

  FILE *stream;
  stream = fopen("/home/fath/.config/termark", "r");
  if (stream == NULL) {
    fprintf(stderr, "Could not open config file");
    exit(EXIT_FAILURE);
  }

  struct Mark *config[10];
  parseConfig(stream, config);

  fclose(stream);
  freeConfig(config, 10);

  return EXIT_SUCCESS;
}

/*
 * The config file format is as follow:
 *
 * markname, markpath,
 * markname, markpath, ...
 *
 * curr_t_type == 0 refers to markname
 * curr_t_type == 1 refers to markpath
 *
 * */
void parseConfig(FILE *stream, struct Mark *config[]) {
  int config_i = 0;

  struct Mark *token = malloc(sizeof(struct Mark));

  int t_i = 0;
  int curr_t_type = 0;

  char curr_char;
  while ((curr_char = fgetc(stream)) != EOF) {
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
        printf("%i. %s: %s\n", config_i, config[config_i]->name,
               config[config_i]->path);

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

  // Handle the last entry if file doesn't end with comma
  if (t_i > 0 && curr_t_type == 1) {
    token->path[t_i] = '\0';
    config[config_i] = malloc(sizeof(struct Mark));
    strcpy(config[config_i]->name, token->name);
    strcpy(config[config_i]->path, token->path);
    printf("%i. %s: %s\n", config_i, config[config_i]->name,
           config[config_i]->path);
  }

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
