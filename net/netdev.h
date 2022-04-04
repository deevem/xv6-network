#ifndef NETDEV_H
#define NETDEV_H

#include "sysheaders.h"
#include "skbuff.h"

struct netdev {
    uint32_t addr;
    uint8_t  addr_len;
    uint8_t  hwaddr[6];
    uint32_t mtu;
};

void netdev_init();
int  netdev_tx(struct sk_buff* skb, uint8_t* dst, uint16_t ethertype);

void* netdev_rx_loop();
void  free_netdev();

struct netdev* netdev_get(uint32_t sip);

#endif