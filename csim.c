#include "cachelab.h"
#include "csim.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>

int main(int argc, char** argv)
{

    options_data options;
    get_options(argc, argv, &options);

    printf("help_flag: %d\nverbose_flag: %d\ns: %d\nE: %d\nb: %d\ntrace_file: %s\n", 
        options.help_flag, options.verbose_flag, options.s, options.E, options.b, options.trace_file);

    size_t inst_count;
    Inst* instructions = parse_trace(options.trace_file, &inst_count);

    for (int i = 0; i < inst_count; ++i) {
        printf("OP: %d, Addr: %08lX\n", instructions[i].operation, instructions[i].address);
    }

    Cache cache = build_cache(options.s, options.E, options.b);
    print_cache(&cache);

    int hits = 0;
    int misses = 0;
    int evictions = 0;

    for (int i = 0; i < inst_count; ++i) {
        uint64_t addr = instructions[i].address;
        uint64_t block_offset = addr & ((1 << cache.b) - 1); // gets lower-order b bits from addr
        uint64_t set_index = (addr >> cache.b) & ((1 << cache.s) - 1); // gets middle s bits from addr
        uint64_t addr_tag = addr >> (cache.b + cache.s); // gets higher-order t = 64 - (s + b) bits from addr
        printf("addr: %04lX, block_offset: %04lX, set_index: %04lX, tag: %04lX\n", addr, block_offset, set_index, addr_tag);
        // Look for line in sets[set_index]
        bool hit = false;
        CacheSet* set = &cache.sets[set_index];
        for (int j = 0; j < cache.E; ++j) {
            if (set->lines[j].valid && set->lines[j].tag == addr_tag) 
                hit = true;
        }
        if (hit) {
            hits++;
        } else {
            misses++;
            // Look for open line to load block into, otherwise evict
        }
    }

    printSummary(hits, misses, evictions);
    return 0;
}

Cache build_cache(int s, int E, int b) 
{
    Cache cache = { .s = s,
                    .E = E,
                    .b = b,
                    .S = 1 << s,   // 2^s
                    .B = 1 << b }; // 2^b
    // Allocate array of sets
    cache.sets = (CacheSet*) malloc(sizeof(CacheSet) * cache.S);
    // For each set allocate an array of lines
    for (int i = 0; i < cache.S; ++i) {
        cache.sets[i].lines = (CacheLine*) malloc(sizeof(CacheLine) * cache.E);
        // Initialize each line in the set
        for (int j = 0; j < E; ++j) {
            cache.sets[i].lines[j] = (CacheLine) { .valid = 0, .tag = 0, .last_used = 0 };
        }
    }
    return cache;
}

void free_cache(Cache* cache) {
    for (int i = 0; i < cache->S; ++i) {
        free(cache->sets[i].lines);
    }
    free(cache->sets);
}

void print_cache(Cache* cache) {
    printf("Cache Size: %d bytes\n", cache->B * cache->E * cache-> S);
    for (int i = 0; i < cache->S; ++i) {
        // print set
        printf("Set %d:\n", i);
        // print lines
        for (int j = 0; j < cache->E; ++j) {
            printf("valid: %d, tag: %08lX, last used: %lu\n",
                    cache->sets[i].lines[j].valid, cache->sets[i].lines[j].tag, cache->sets[i].lines[j].last_used);
        }
        printf("\n");
    }
}

Inst* parse_trace(char* filename, size_t* inst_count)
{
    // Open file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return NULL;
    }

    // Count number of data instructions and allocate instructions array
    char line[64];  // Trace lines can only get to like 21 bytes
    *inst_count = 0;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'I') continue;
        if (line[1] == 'M') {
            *inst_count += 2; // Modify is a load and store instruction
        } else {
            *inst_count += 1;
        }
    }

    Inst* instructions = (Inst*) malloc(sizeof(Inst) * (*inst_count));

    // Add each data instructions into the IR
    size_t inst_idx = 0;
    rewind(file); // Reset file pointer to beginning
    while (fgets(line, sizeof(line), file)) {
        if (line[0] == 'I') continue;

        // Gets address from line
        char* address_start = &line[3];
        uint64_t address = (uint64_t) strtol(address_start, NULL, 16);  // base 16 for hex

        // Adds corresponding instuction(s) to IR
        char op = line[1];
        if (op == 'M') {
            instructions[inst_idx++] = (Inst) {.operation = LOAD, .address = address}; // Load inst, then
            instructions[inst_idx++] = (Inst) {.operation = STORE, .address = address}; // store inst
        } else if (op == 'L') {
            instructions[inst_idx++] = (Inst) {.operation = LOAD, .address = address}; // Load inst
        } else {
            instructions[inst_idx++] = (Inst) {.operation = STORE, .address = address}; // Store inst
        }
    }

    fclose(file);
    return instructions;
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
