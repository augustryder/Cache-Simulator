#include "cachelab.h"
#include "csim.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

int main(int argc, char** argv)
{
    printSummary(0, 0, 0);

    options_data options;
    get_options(argc, argv, &options);

    printf("help_flag: %d\nverbose_flag: %d\ns: %d\nE: %d\nb: %d\ntrace_file: %s\n", 
        options.help_flag, options.verbose_flag, options.s, options.E, options.b, options.trace_file);

    const int ADDRESS_BITS = 64; // assume 64 bit system

    // Initializing necessary quantities
    const int s = options.s; // number of set bits
    const int E = options.E; // associativity, number of lines per set
    const int b = options.b; // number of block offset bits
    const int t = ADDRESS_BITS - (s + b); // number of tag bits
    const int block_size = pow(2, b);
    const int num_sets = pow(2, s);
    const int cache_size = num_sets * E * block_size;

    return 0;
}

Inst* parse_trace(char* filename)
{
    // Open file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    // Count number of data instructions and allocate instructions array
    char line[64];  // Trace lines can only get to like 21 bytes
    int inst_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'I') continue;
        if (line[1] == 'M') {
            inst_count += 2; // Modify is a load and store instruction
        } else {
            inst_count += 1;
        }
    }

    Inst* instructions = (Inst*) malloc(sizeof(Inst) * inst_count);

    // Adds data instructions to IR

    fclose(file);
    return NULL;
}


options_data* get_options(int argc, char** argv, options_data* options) 
{
    options->help_flag = 0;
    options->verbose_flag = 0;
    options->s = 0;
    options->E = 0;
    options->b = 0;
    options->trace_file = NULL;
  
    int c;
    while ((c = getopt(argc, argv, "hvs:E:b:t:")) != -1) 
    {
        switch (c) 
        {
            case 'h':
                options->help_flag = 1;
                break;
            case 'v':
                options->verbose_flag = 1;
                break;
            case 's':
                options->s = atoi(optarg);
                break;
            case 'E':
                options->E = atoi(optarg);
                break;
            case 'b':
                options->b = atoi(optarg);
                break;
            case 't':
                options->trace_file = optarg;
                break;
            case '?':
                fprintf(stderr, "Error parsing options.\n");
                return NULL;
            default:
                abort();
        }
    }
    return NULL;
}
