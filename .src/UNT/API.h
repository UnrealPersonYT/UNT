/// @file  UNT/API.h
/// @brief Contains a simple UNT api
#ifndef _UNT_API_H
#define _UNT_API_H

#include "stdint.h"

/// @brief     Computes total bytes needed to hold UNT-encoded data
/// @param len Size of raw input buffer
/// @return    Total UNT bytes needed
static inline word untfslen(word len){
    return ((len * 8u) + 5u) / 6u;
}
/// @brief     Computes total bytes produced when decoding UNT data
/// @param len Size of UNT buffer
/// @return    Total raw bytes produced
static inline word unttslen(word len){
    return (len * 3u) / 4u;
}
/// @brief     Converts raw bytes to UNT
/// @param in  Input buffer
/// @param len Input size
/// @param out Output buffer (must be >= untfslen(len))
/// @return    Total bytes written to out
static inline word untfs(const u8* __restrict in, const word len, u8* __restrict out){
    word i = 0;     // input index
    word oi = 0;    // output index
    word acc = 0;   // bit accumulator
    word bits = 0;  // number of bits in accumulator

    for(; i < len; ++i){
        acc = (acc << 8) | in[i];
        bits += 8;

        while(bits >= 6){
            bits -= 6;
            u8 chunk = (acc >> bits) & 0x3F;
            out[oi++] = 0xC0 | chunk;
        }
    }

    if(bits > 0){
        u8 chunk = (acc << (6 - bits)) & 0x3F;
        out[oi++] = 0x80 | chunk;
    }else if(oi > 0)
        out[oi - 1] = (out[oi - 1] & 0x3F) | 0x80;

    return oi;
}
/// @brief      Converts UNT data back to raw bytes
/// @param unt UNT buffer
/// @param len  UNT size
/// @param out  Output buffer (must be >= unttslen(len))
/// @return     Total bytes written to out
static inline word untts(const u8* __restrict unt, const word len, u8* __restrict out){
    word i = 0;     // input index
    word oi = 0;    // output index
    word acc = 0;   // bit accumulator
    word bits = 0;  // number of bits in accumulator

    for( ; i < len; ++i){
        u8 b = unt[i];

        if(!(b & 0x80))
            break;

        u8 tag = b >> 6;
        u8 chunk = b & 0x3F;

        acc = (acc << 6) | chunk;
        bits += 6;

        while(bits >= 8){
            bits -= 8;
            out[oi++] = (acc >> bits) & 0xFF;
        }

        if(tag == 2)
            break;
    }
    return oi;
}

#endif  // _UNT_API_H