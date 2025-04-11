#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <stdint.h>

typedef struct {
    int help_flag;
    int verbose_flag;
    int s;
    int E;
    int b;
    char* trace_file;
} options_data;

typedef enum {
    LOAD,
    STORE
} OpType;

typedef struct {
    OpType operation;
    uint64_t address;
} Inst;

// Parses options and puts them in a options_data struct
options_data* get_options(int argc, char** argv, options_data* options);
// Parses trace file and returns an array of memory addresses
Inst* parse_trace(char* filename);
