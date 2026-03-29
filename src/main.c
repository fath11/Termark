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
  char *config_path = "/.config/termark/marks";
  char config_dir[PATH_MAX];
  snprintf(config_dir, sizeof(config_dir), "%s%s", home_path, config_path);

  char cwd[PATH_MAX];
  if (getcwd(cwd, sizeof cwd) == NULL) {
    perror("getcwd() error");
    exit(EXIT_FAILURE);
  }

  FILE *config_stream;
  struct Mark *config[MAX_CONFIG_SIZE];
  int config_len;
  config_stream = fopen(config_dir, "r+");
  if (config_stream == NULL) {
    fprintf(stderr, "Could not read config file\n");
    exit(EXIT_FAILURE);
  }
  parseConfig(config_stream, config, &config_len);

  int opt;
  while ((opt = getopt(argc, argv, "ln:d:")) != -1) {
    if (opt == 'l') {
      printf("Listing all marks (%d/%d) -----------\n", config_len,
             MAX_CONFIG_SIZE);
      printf("  index    name     path\n");
      for (int i = 0; i < config_len; i++) {
        printf("   %i        %s     %s\n", i + 1, config[i]->name,
               config[i]->path);
      }
      printf("-------------------------------------\n");
    }

    else if (opt == 'n') {
      fclose(config_stream);
      config_stream = fopen(config_dir, "w");
      if (config_stream == NULL) {
        fprintf(stderr, "Could not write config file\n");
        exit(EXIT_FAILURE);
      }

      printf("Created new mark called %s for directory %s\n", optarg, cwd);
      fprintf(config_stream, "%s,%s,\n", optarg, cwd);
      for (int i = 0; i < config_len; i++) {
        fprintf(config_stream, "%s,%s,\n", config[i]->name, config[i]->path);
      }
    }

    else if (opt == 'd') {
      fclose(config_stream);
      config_stream = fopen(config_dir, "w");
      if (config_stream == NULL) {
        fprintf(stderr, "Could not write config file\n");
        exit(EXIT_FAILURE);
      }

      for (int i = 0; i < config_len; i++) {
        if (strcmp(config[i]->name, optarg) == 0) {
          continue;
        }
        fprintf(config_stream, "%s,%s,\n", config[i]->name, config[i]->path);
      }
    }

    else if (opt == '?') {
      exit(EXIT_FAILURE);
    }
  }

  // No flag is provided
  if (optind == 1) {
    for (int i = 0; i < config_len; i++) {
      if (strcmp(config[i]->name, argv[1]) == 0) {
          printf("%s", config[i]->path);
        break;
      }
    }
  }

  return EXIT_SUCCESS;
}
