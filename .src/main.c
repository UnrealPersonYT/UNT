#define _CRT_SECURE_NO_WARNINGS
#include "UNT/API.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

int main(int argc, char** argv){
    if(argc < 2){
        printf("Usage:\n");
        printf("  %s [-e|-d] <input> [output]\n", argv[0]);
        return 1;
    }

    int mode = 0; // 0 = encode, 1 = decode
    const char* infile = NULL;
    const char* outfile = NULL;
    char auto_out[1024];

    // Parse args in any order
    for(int i = 1; i < argc; ++i){
        if(strcmp(argv[i], "-e") == 0){
            mode = 0;
        }else if(strcmp(argv[i], "-d") == 0){
            mode = 1;
        }else if(!infile){
            infile = argv[i];
        }else if(!outfile){
            outfile = argv[i];
        }
    }

    if(!infile){
        printf("Error: no input file provided.\n");
        return 1;
    }

    // Auto-generate output name if not provided
    if(!outfile){
        if(mode == 0){
            snprintf(auto_out, sizeof(auto_out), "%s.unt", infile);
        }else{
            snprintf(auto_out, sizeof(auto_out), "%s.raw", infile);
        }
        outfile = auto_out;
    }

    clock_t start = clock();

    // Open input file
    FILE* f = fopen(infile, "rb");
    if(!f){
        printf("Error: cannot open input file.\n");
        return 1;
    }

    // Determine file size
    fseek(f, 0, SEEK_END);
    word insize = ftell(f);
    fseek(f, 0, SEEK_SET);

    // Allocate input buffer
    u8* inbuf = malloc(insize);
    if(!inbuf){
        printf("Error: cannot allocate input buffer.\n");
        fclose(f);
        return 1;
    }

    fread(inbuf, 1, insize, f);
    fclose(f);

    u8* outbuf;
    word outsize;

    if(mode == 0){
        // Encode
        outsize = untfslen(insize);
        outbuf = malloc(outsize);
        if(!outbuf){
            printf("Error: cannot allocate output buffer.\n");
            free(inbuf);
            return 1;
        }

        word produced = untfs(inbuf, insize, outbuf);

        printf("Encoding...\n");
        printf("Input size:   %llu bytes\n", (unsigned long long)insize);
        printf("UNT size:    %llu bytes\n", (unsigned long long)outsize);
        printf("Output size:  %llu bytes\n", (unsigned long long)produced);

        // shrink to actual produced size
        outsize = produced;

    }else{
        // Decode
        outsize = unttslen(insize);
        outbuf = malloc(outsize);
        if(!outbuf){
            printf("Error: cannot allocate output buffer.\n");
            free(inbuf);
            return 1;
        }

        word consumed = untts(inbuf, insize, outbuf);

        printf("Decoding...\n");
        printf("Input size:   %llu bytes\n", (unsigned long long)insize);
        printf("Consumed:     %llu bytes\n", (unsigned long long)consumed);
        printf("Output size:  %llu bytes\n", (unsigned long long)consumed);
    }

    // Write output file
    FILE* o = fopen(outfile, "wb");
    if(!o){
        printf("Error: cannot open output file.\n");
        free(inbuf);
        free(outbuf);
        return 1;
    }

    fwrite(outbuf, 1, outsize, o);
    fclose(o);

    printf("Wrote file: %s\n", outfile);

    clock_t end = clock();
    double elapsed = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Total time: %.6f sec\n", elapsed);

    free(inbuf);
    free(outbuf);
    return 0;
}