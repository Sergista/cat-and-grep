#ifndef S21_GREP_H
#define S21_GREP_H

#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER 1000

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int o;
  int f;
  int c_flag;
  char pattern_arguments[BUFFER];
  char str[BUFFER];
  char matched_str[BUFFER];
  char filename[BUFFER];
} flags;

void parser(int argc, char *argv[], flags *FLAG);
void output(char *argv[], flags *FLAG);
void f_flag(flags *FLAG);

#endif  // S21_GREP_H