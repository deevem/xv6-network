
#include "netdev.h"
#include "sysheaders.h"

// header of ethernet protocal

#define ETHADDR_LEN 6
#define ETH_HDR_LEN sizeof(struct eth_hdr)

// values of ip_hdr.ip_p

#define IPPROTO_ICMP 1  // Control message protocol
#define IPPROTO_TCP 6   // Transmission control protocol
#define IPPROTO_UDP 17  // User datagram protocol

struct eth_hdr {
    uint8_t dsthost[ETHADDR_LEN];
    uint8_t srchost[ETHADDR_LEN];
    uint16_t type;
    uint8_t payload[];
}__attribute__((packed));
