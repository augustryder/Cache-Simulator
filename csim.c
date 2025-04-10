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
