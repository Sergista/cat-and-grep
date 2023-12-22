#include <getopt.h>
#include <stdio.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
  int error;
} flags;

void parser(flags *main_flags, int argc, char **argv);
void cat(flags *main_flags, const char *filename);