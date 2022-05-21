#pragma once
// This header file stores many useful and often used system headers, all commented

// uint types
#include "../types.h"
#include "../param.h"
#include "../riscv.h"
#include "../defs.h"

static inline uint16 bswaps(uint16 val)
{
  return (((val & 0x00ffU) << 8) |
          ((val & 0xff00U) >> 8));
}

static inline uint32 bswapl(uint32 val)
{
  return (((val & 0x000000ffUL) << 24) |
          ((val & 0x0000ff00UL) << 8) |
          ((val & 0x00ff0000UL) >> 8) |
          ((val & 0xff000000UL) >> 24));
}

#define ntohs bswaps
#define ntohl bswapl
#define htons bswaps
#define htonl bswapl

#define NULL 0