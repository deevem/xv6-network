#ifndef IP
#define IP

#include "ethernet.h"
#include "sysheaders.h"
#include "skbuff.h"

struct ip_hdr {
    uint8_t  ihl = 4;        // Internet Header Length
    uint8_t  verison = 4;
    uint8_t  tos;            // type of service
    uint16_t len;
    uint16_t id;             // identification
    uint16_t frag_offset;
    uint8_t  ttl;            // time to live
    uint8_t  protocal;
    uint16_t checksum;
    uint32_t src_addr;       // source address
    uint32_t dst_addr;       // destination ip address
    uint8_t  data[];
} __attribute__((packed));


#define IP_HDR_LEN sizeof(struct ip_hdr)


int ip_rx(struct sk_buff *skb);
void ip_tx(struct sock* sk, struct sk_buff *skb);


#endif