#include "s21_cat.h"

int main(int argc, char **argv) {
  flags main_flags = {0};
  parser(&main_flags, argc, argv);

  if (argc == optind) {
    cat(&main_flags, NULL);
  } else {
    while (optind < argc) {
      cat(&main_flags, argv[optind++]);
    }
  }
  return 0;
}

void parser(flags *main_flags, int argc, char **argv) {
  int result = 0;
  const char *short_options = "+benstv";
  const struct option long_options[] = {{"number-nonblank", 0, 0, 'b'},
                                        {"number", 0, 0, 'n'},
                                        {"squeeze-blank", 0, 0, 's'},
                                        {0, 0, 0, 0}};

  while ((result = getopt_long(argc, argv, short_options, long_options,
                               NULL)) != -1) {
    switch (result) {
      case 'b':
        main_flags->b = 1;
        break;

      case 'e':
        main_flags->e = 1;
        main_flags->v = 1;
        break;

      case 'E':
        main_flags->e = 1;
        break;

      case 'n':
        main_flags->n = 1;
        break;

      case 's':
        main_flags->s = 1;
        break;

      case 't':
        main_flags->t = 1;
        main_flags->v = 1;
        break;
      case 'T':
        main_flags->t = 1;
        break;

      case 'v':
        main_flags->v = 1;
        break;

      case '?':
        main_flags->error = 1;
        break;
    }
  }

  if (main_flags->b && main_flags->n) {
    main_flags->n = 0;
  }
}

void cat(flags *main_flags, const char *filename) {
  FILE *fp = NULL;

  if (filename == NULL) {
    fp = stdin;
  } else {
    fp = fopen(filename, "r");
    if (fp == NULL) {
      fprintf(stderr, "s21_cat: %s: No such file or directory\n", filename);
      return;
    }
  }

  int line_count = 1;
  int empty_line_count = 0;
  char prev_c = '\n';

  while (1) {
    int c = fgetc(fp);
    if (c == EOF) {
      break;
    }

    if (main_flags->s && c == '\n' && prev_c == '\n') {
      empty_line_count++;
      if (empty_line_count > 1) {
        continue;
      }
    } else {
      empty_line_count = 0;
    }

    if (prev_c == '\n' && ((main_flags->b && c != '\n') || main_flags->n)) {
      printf("%6d\t", line_count++);
    }

    if (main_flags->t && c == '\t') {
      printf("^");
      c = 'I';
    }

    if (main_flags->e && c == '\n') {
      printf("$");
    }

    if (c != '\n' && main_flags->v) {
      if (c < 32 && c != 9 && c != 10) {
        printf("^");
        c += 64;
      } else if (c > 127 && c < 160) {
        printf("M-^");
        c -= 64;
      } else if (c == 127) {
        printf("^");
        c -= 64;
      }
    }

    printf("%c", c);
    prev_c = c;
  }

  if (filename != NULL) {
    fclose(fp);
  }
}
