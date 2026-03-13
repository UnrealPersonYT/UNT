/// @file  UNT/API.h
/// @brief Contains a simple UNT api
#ifndef _UNT_API_H
#define _UNT_API_H

#include "stdint.h"

#define UNT_INVALID (u8)0x80

/// @brief     Computes total bytes needed to hold UNT-encoded data
/// @param len Size of raw input buffer
/// @return    Total UNT bytes needed
static inline word untenclen(word len){
    return ((len * 8u) + 5u) / 6u;
}
/// @brief     Computes total bytes produced when decoding UNT data
/// @param len Size of UNT buffer
/// @return    Total raw bytes produced
static inline word untdeclen(word len){
    return (len * 3u) / 4u;
}
/// @brief     Converts raw bytes to UNT
/// @param in  Input buffer
/// @param len Input size
/// @param out Output buffer (must be >= untfslen(len))
/// @return    Total bytes written to out
static inline word untenc(const u8* __restrict in, const word len, u8* __restrict out){
    word o = 0;
    for(word i = 0; i < len; i += 3){
        u32 val = 0;
        const word limit = ((len - i) > 3) ? 3 : (len - i);
        for(word v = 0; v < limit; ++v)
            val |= (u32)in[i + v] << ((2 - v) * 8);
        for(word b = 0; b < limit + 1; ++b)
            out[o++] = 0xC0 | ((val >> (18 - (b * 6))) & 0x3F);
    }
    // End last output
    if(o)
        out[o - 1] &= 0xBF;
    return o;
}
/// @brief     Converts UNT data back to raw bytes
/// @param in  UNT buffer
/// @param len UNT size
/// @param out Output buffer (must be >= unttslen(len))
/// @return    Total bytes written to out
static inline word untdec(const u8* __restrict in, const word len, u8* __restrict out){
    word o = 0;
    for(word i = 0; i < len; i += 4){
        u32 val = 0;
        const word limit = ((len - i) > 4) ? 4 : (len - i);
        for(word v = 0; v < limit; ++v){
            u8 x = in[i + v];
            if(x & UNT_INVALID)
                return o;
            val |= (u32)x << (18 - (v * 6));
        }
        for(word b = 0; b < (limit - 1); ++b)
            out[o++] = (val >> (16 - (b * 8))) & 0xFF;
    }
    return o;
}

#endif  // _UNT_API_H