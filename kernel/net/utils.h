#ifndef UTILS
#define UTILS

#include "sysheaders.h"
#include "mbuf.h"

uint16_t checksum(const unsigned char* addr, int len);

uint16_t tcp_checksum(struct mbuf* m, uint32_t saddr, uint32_t daddr);

int min(int a, int b) {
    return a < b ? a : b;
}

#endif