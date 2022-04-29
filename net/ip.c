#include "ip.h"
#include "ethernet.h"

void ip_tx(struct mbuf *m, uint8_t protocol, uint32_t dst_ip) {
    struct ip_hdr *iphdr;

    iphdr = (struct ip_hdr *)mbufpush(m, sizeof(struct ip_hdr));
    memset(iphdr, 0, sizeof iphdr);
    iphdr->vhl = (4 << 4) | (20 >> 2);
    iphdr->protocol = protocol;
    
    // TODO: local ip
    iphdr->src_addr = htonl(MAKE_IP_ADDR(10, 1, 1, 2));
    iphdr->dst_addr = htonl(dst_ip);
    iphdr->len = htons(m->len);
    iphdr->ttl = 100;
    // TODO: Checksum
    iphdr->checksum = 0;

    printf("ip ready for tx\n");

    eth_tx(m, ETHTYPE_IP);
}

void ip_rx(struct mbuf* m) {
    // TODO error process

    struct ip_hdr *iphdr;
    uint16_t len;

    iphdr = (struct ip_hdr*)mbufpull(m, sizeof(struct ip_hdr));

    len = ntohs(iphdr->len) - sizeof(struct ip_hdr);
    printf("%s %d\n", "ip protocol", iphdr->protocol);
    if (iphdr->protocol == IPPROTO_UDP)
        udp_rx(m, len, iphdr);
    else 
        mbuffree(m);
    return;
}