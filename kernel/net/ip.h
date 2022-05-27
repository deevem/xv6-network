#ifndef IP
#define IP

#include "ethernet.h"
#include "sysheaders.h"
#include "mbuf.h"

struct ip_hdr {
    uint8_t  vhl;
    uint8_t  tos;            // type of service
    uint16_t len;
    uint16_t id;             // identification
    uint16_t frag_offset;
    uint8_t  ttl;            // time to live
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t src_addr;       // source address
    uint32_t dst_addr;       // destination ip address
};


#define IP_HDR_LEN sizeof(struct ip_hdr)

// values of ip_hdr.protocol

#define IPPROTO_ICMP 1  // Control message protocol
#define IPPROTO_TCP 6   // Transmission control protocol
#define IPPROTO_UDP 17  // User datagram protocol


void ip_rx(struct mbuf *m);
void ip_tx(struct mbuf *m, uint8_t protocol, uint32_t dst_ip);
void ip_tx_ready(struct mbuf* m);

#define local_ip MAKE_IP_ADDR(10, 0, 2, 15)

#endif