#include "s21_grep.h"

int main(int argc, char *argv[]) {
  flags FLAG = {0};
  parser(argc, argv, &FLAG);
  while (optind < argc) {
    output(argv, &FLAG);
    optind++;
  }
  return 0;
}

void parser(int argc, char *argv[], flags *FLAG) {
  int option;
  while ((option = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, 0)) != -1) {
    switch (option) {
      case 'e':
        FLAG->e = 1;
        strcat(FLAG->pattern_arguments, optarg);
        strcat(FLAG->pattern_arguments, "|");
        break;
      case 'i':
        FLAG->i = 1;
        break;
      case 'v':
        FLAG->v = 1;
        break;
      case 'c':
        FLAG->c = 1;
        break;
      case 'l':
        FLAG->l = 1;
        break;
      case 'n':
        FLAG->n = 1;
        break;
      case 'h':
        FLAG->h = 1;
        break;
      case 's':
        FLAG->s = 1;
        break;
      case 'o':
        FLAG->o = 1;
        break;
      case 'f':
        FLAG->f = 1;
        strcpy(FLAG->filename, optarg);
        f_flag(FLAG);
        break;
      default:
        break;
    }
    if (FLAG->v && FLAG->o) {
      FLAG->o = 0;
    }
  }
  if (!FLAG->e && !FLAG->f) {
    if (argc > optind) {
      strcat(FLAG->pattern_arguments, argv[optind]);
    }
    optind++;
  }
  if (FLAG->e || FLAG->f) {
    FLAG->pattern_arguments[strlen(FLAG->pattern_arguments) - 1] = '\0';
  }
  if (argc - optind > 1) FLAG->c_flag = 1;
}

void output(char *argv[], flags *FLAG) {
  FILE *file;
  regex_t reg;
  regmatch_t start;
  int counter = 0;
  int str_number = 0;
  int flag_i = REG_EXTENDED;
  if (FLAG->i) {
    flag_i = REG_EXTENDED | REG_ICASE;
  }
  regcomp(&reg, FLAG->pattern_arguments, flag_i);
  file = fopen(argv[optind], "rb");
  if (file != NULL) {
    while (fgets(FLAG->str, BUFFER, file) != NULL) {
      int match = regexec(&reg, FLAG->str, 1, &start, 0);
      str_number++;
      if (FLAG->o) strcpy(FLAG->matched_str, FLAG->str);
      if (((!match && !FLAG->v) || (FLAG->v && match)) && FLAG->c_flag &&
          !FLAG->l && !FLAG->h && !FLAG->c)
        printf("%s:", argv[optind]);
      if (!match) counter++;
      if (FLAG->v) match = !match;
      if (!match && !FLAG->l && !FLAG->c && !FLAG->n && !FLAG->o) {
        printf("%s", FLAG->str);
        if (FLAG->str[strlen(FLAG->str) - 1] != '\n') printf("\n");
      }
      if (!match && FLAG->n && !FLAG->c && !FLAG->l) {
        if (FLAG->o) {
          printf("%d:", str_number);
        } else
          printf("%d:%s", str_number, FLAG->str);
        if (FLAG->str[strlen(FLAG->str) - 1] != '\n') printf("\n");
      }
      if (!match && FLAG->o && !FLAG->l && !FLAG->c) {
        char *pointer = FLAG->matched_str;
        while ((regexec(&reg, pointer, 1, &start, 0) == 0)) {
          printf("%.*s\n", (int)(start.rm_eo - start.rm_so),
                 pointer + start.rm_so);
          pointer += start.rm_eo;
        }
      }
    }
    if (FLAG->l && counter < 1 && FLAG->v) {
      printf("%s\n", argv[optind]);
    }
    if (FLAG->l && counter > 0 && !FLAG->c) printf("%s\n", argv[optind]);
    if (FLAG->c && FLAG->c_flag && !FLAG->h) printf("%s:", argv[optind]);
    if (FLAG->c && !FLAG->l && !FLAG->v) printf("%d\n", counter);
    if (FLAG->c && !FLAG->l && FLAG->v) printf("%d\n", str_number - counter);
    if (FLAG->c && FLAG->l) {
      if (counter > 0) {
        counter = 1;
        printf("%d\n%s\n", counter, argv[optind]);
      } else
        printf("%d\n", counter);
    }
    fclose(file);
  } else {
    if (!FLAG->s)
      fprintf(stderr, "grep: %s: No such file or directory\n", argv[optind]);
  }
  regfree(&reg);
}

void f_flag(flags *FLAG) {
  FILE *f;
  f = fopen(FLAG->filename, "rb");
  if (f != NULL) {
    while (fgets(FLAG->str, 1000, f) != NULL) {
      if (FLAG->str[strlen(FLAG->str) - 1] == '\n' &&
          strlen(FLAG->str) - 1 != 0)
        FLAG->str[strlen(FLAG->str) - 1] = '\0';
      strcat(FLAG->pattern_arguments, FLAG->str);
      strcat(FLAG->pattern_arguments, "|");
    }
    fclose(f);
  }
}
