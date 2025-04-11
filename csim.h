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

typedef struct {
    int valid; // valid bit, either 0 and 1
    uint64_t tag; // integer representation of tag bits
    uint64_t last_used; // index of last instruction this line was used for
} CacheLine;

typedef struct {
    CacheLine* lines;
} CacheSet;

typedef struct {
    int s; // number of set bits
    int E; // associativity, number of lines per set
    int b; // number of block bits
    int S; // number of sets, 2^s (1 << s)
    int B; // block size, 2^b (1 << b)
    CacheSet* sets;
} Cache;

options_data* get_options(int argc, char** argv, options_data* options); // Parses options and puts them in a options_data struct
Inst* parse_trace(char* filename); // Parses trace file and returns an array of memory addresses
Cache build_cache(int s, int E, int b); // Allocates a cache (s, E, b) and returns a pointer to it
void free_cache(Cache* cache); // Frees dynamically allocated cache arrays
void print_cache(Cache* cache); // Prints the size and contents of the cache
