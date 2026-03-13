#include "UNT/API.h"
#ifdef _WIN32
    #define _CRT_SECURE_NO_WARNINGS
    #include <io.h>
    #include <fcntl.h>
#endif
#include <string.h>
#include <stdio.h>
#define INPUT_SIZE (4096 * 3)

u8 inp_buf[INPUT_SIZE];
u8 out_buf[((INPUT_SIZE * 4) + 2) / 3];

i32 main(i32 argc, char* argv[]){
#ifdef _WIN32
    _setmode(_fileno(stdin),  _O_BINARY);
    _setmode(_fileno(stdout), _O_BINARY);
#endif
    word read, overflow = 0, mode = 1;
    if(argc > 1 && !strcmp(argv[1], "-d"))
        mode = 0;
    while((read = fread(&inp_buf[overflow], 1, INPUT_SIZE - overflow, stdin))){
        overflow += read;
        if(!(overflow % 3)){
            fwrite(out_buf, 1, mode ? untenc(inp_buf, overflow, out_buf): untdec(inp_buf, overflow, out_buf), stdout);
            overflow = 0;
        }
    }
    if(overflow)
        fwrite(out_buf, 1, mode ? untenc(inp_buf, overflow, out_buf): untdec(inp_buf, overflow, out_buf), stdout);
    return 0;
}