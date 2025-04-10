#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

typedef struct options {
    int help_flag;
    int verbose_flag;
    int s;
    int E;
    int b;
    char* trace_file;
} options_data;

// Parses options and puts them in a options_data struct
options_data* get_options(int argc, char** argv, options_data* options);
