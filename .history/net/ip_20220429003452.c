#include "ip.h"
#include "ethernet.h"

void ip_tx(struct mbuf *m, uint8_t protocal, uint32_t dst_ip) {
    struct ip_hdr *iphdr;

    iphdr = (struct ip_hdr *)mbufpush(m, sizeof(struct ip_hdr));
    memset(iphdr, 0, sizeof iphdr);
    iphdr->verison = (4 << 4) | (20 >> 2);
    iphdr->protocal = protocal;
    
    // TODO: local ip
    // iphdr->src_addr = htonl(local_ip);
    iphdr->dst_addr = htonl(dst_ip);
    iphdr->len = htons(m->len);
    iphdr->ttl = 100;
    // TODO: Checksum
    iphdr->checksum = 0;

    eth_tx(m, ETHTYPE_IP);
}
