#include "udp.h"
#include "mbuf.h"
#include "ip.h"

void udp_tx(struct mbuf *m, uint32_t dst_ip, uint16_t src_port, uint16_t dst_port) {
    struct udp_hdr *udphdr;

    udphdr = (struct udp_hdr*)mbufpush(m, sizeof(struct udp_hdr));
    printf("%s", "mbufpush finished in udp tx\n");


    udphdr->src_port = htons(src_port);
    udphdr->dst_port = htons(dst_port);
    udphdr->len      = htons(m->len);
    // TODO: now no checksum
    udphdr->checksum = 0;

    ip_tx(m, IPPROTO_UDP, dst_ip);
}


int udp_rx(struct mbuf *m, uint16_t len, struct ip_hdr *iphdr) {
    struct udp_hdr *udphdr;
    uint32_t src_ip;
    uint16_t src_port, dst_port;

    // get udp hdr from m

    if (udphdr == NULL) {
        // free the m
        goto fail;
    }

    if (ntohs(udphdr->len) != len)
        goto fail;

    len -= sizeof(*udphdr);
    
    if (len > m->len)
        goto fail;

    src_ip = ntohl(iphdr->src_addr);
    src_port = ntohs(udphdr->src_port);
    dst_port = ntohs(udphdr->dst_port);
    
    // recieve the udp packet by the socket

fail:
    return;
}
