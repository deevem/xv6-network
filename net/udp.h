#ifndef UDP
#define UDP

#include "sysheaders.h"
#include "skbuff.h"

struct udp_hdr {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t len;
    uint16_t checksum;
};

void udp_tx(struct sk_buff *skb, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port);

int udp_rx(struct sk_buff *skb, uint16_t len, struct ip_hdr *iphdr);



#endif