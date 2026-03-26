#include <linux/limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../include/configparser.h"
#include "../include/const.h"
#include "../include/types.h"

int main(int argc, char *argv[]) {
  if (argc <= 1) {
    fprintf(stderr, "Please provide an argument\n");
    exit(EXIT_FAILURE);
  }

  char *home_path = getenv("HOME");
  int home_path_len = strlen(home_path);

  char *config_path = "/.config/termark";
  int config_path_len = strlen(config_path);

  int config_dir_len = home_path_len + config_path_len;
  char config_dir[config_dir_len];

  strcat(config_dir, home_path);
  strcat(config_dir, config_path);

  FILE *stream;
  stream = fopen(config_dir, "r+");
  if (stream == NULL) {
    fprintf(stderr, "Could not open config file\n");
    exit(EXIT_FAILURE);
  }

  struct Mark *config[MAX_CONFIG_SIZE];
  int config_len;
  parseConfig(stream, config, &config_len);

  int opt;
  while ((opt = getopt(argc, argv, "ln")) != -1) {
    if (opt == 'l' && config_len > 0) {
      printf("Listing all marks (%d/%d) -----------\n", config_len,
             MAX_CONFIG_SIZE);
      printf("  index    name     path\n");
      for (int i = 0; i < config_len; i++) {
        printf("   %i        %s     %s\n", i + 1, config[i]->name,
               config[i]->path);
      }
      printf("-------------------------------------\n");
    }

    if (opt == 'n') {
      char cwd[PATH_MAX];
      if (getcwd(cwd, sizeof cwd) == NULL) {
        perror("getcwd() error");
        exit(EXIT_FAILURE);
      }

      char markname[PATH_MAX];
      if (sizeof(argv[2]) <= sizeof markname) {
        strcpy(markname, argv[2]);
      } else {
        fprintf(stderr, "Mark name is too long, maximum %i bytes\n", PATH_MAX);
      }

      printf("Created new mark called %s for directory %s\n", markname, cwd);
      fprintf(stream, "%s,%s,\n", markname, cwd);
    }

    if (opt == '?') {
      exit(EXIT_FAILURE);
    }
  }

  fclose(stream);
  freeConfig(config, config_len);

  return EXIT_SUCCESS;
}
