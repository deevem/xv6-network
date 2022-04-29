#ifndef UDP
#define UDP

#include "sysheaders.h"
#include "ip.h"
#include "mbuf.h"

struct udp_hdr {
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t len;
    uint16_t checksum;
};

void udp_tx(struct mbuf *m, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port);

void udp_rx(struct mbuf *m, uint16_t len, struct ip_hdr *iphdr);



#endif