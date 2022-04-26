
#include "netdev.h"
#include "sysheaders.h"

// header of ethernet protocal

#define ETHADDR_LEN 6
#define ETH_HDR_LEN sizeof(struct eth_hdr)

#define ETHTYPE_IP  0x0800 // Internet protocol
#define ETHTYPE_ARP 0x0806 // Address resolution protocol


struct eth_hdr {
    uint8_t dsthost[ETHADDR_LEN];
    uint8_t srchost[ETHADDR_LEN];
    uint16_t type;
    uint8_t payload[];
}__attribute__((packed));


static void eth_tx(struct sk_buff *skb, uint16_t ethType);