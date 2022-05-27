#ifndef ETHERNET
#define ETHERNET

#include "sysheaders.h"
#include "mbuf.h"
#include "e1000_dev.h"
// header of ethernet protocal

#define ETHADDR_LEN 6
#define ETH_HDR_LEN sizeof(struct eth_hdr)

#define ETHTYPE_IP  0x0800 // Internet protocol
#define ETHTYPE_ARP 0x0806 // Address resolution protocol

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

struct eth_hdr {
    uint8_t dsthost[ETHADDR_LEN];
    uint8_t srchost[ETHADDR_LEN];
    uint16_t type;
}__attribute__((packed));


void eth_tx(struct mbuf *m, uint16_t ethType);

void eth_rx(struct mbuf *m);
#endif