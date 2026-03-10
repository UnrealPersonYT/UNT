/// @file  UNT/stdint.h
/// @brief Contains safe type abstractions
#ifndef _UNT_STDINT_H
#define _UNT_STDINT_H

#include <stdint.h>
#include <stddef.h>

// Floating point

typedef double f64; // 64-Bit floating point
typedef float  f32; // 32-Bit floating point

// Integer

typedef int64_t  i64;   // 64-Bit signed   integer
typedef uint64_t u64;   // 64-Bit unsigned integer
typedef int32_t  i32;   // 32-Bit signed   integer
typedef uint32_t u32;   // 32-Bit unsigned integer
typedef int16_t  i16;   // 16-Bit signed   integer
typedef uint16_t u16;   // 16-Bit unsigned integer
typedef int8_t   i8;    // 8-Bit  signed   integer
typedef uint8_t  u8;    // 8-Bit  unsigned integer

// Architecture native

typedef intptr_t  iword;    // Signed   architecture native word
typedef uintptr_t word;     // Unsigned architecture native word

// Generics

typedef void* addr; // Address to memory region
typedef i32   err;  // Generic error code

#endif  // _UNT_STDINT_H