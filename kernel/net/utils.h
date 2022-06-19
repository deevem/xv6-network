#ifndef UTILS
#define UTILS

#include "sysheaders.h"

uint16_t checksum(const unsigned char* addr, int len);

int min(int a, int b) {
    return a < b ? a : b;
}

#endif