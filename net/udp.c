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


void udp_rx(struct mbuf *m, uint16_t len, struct ip_hdr *iphdr) {
    struct udp_hdr *udphdr;
    uint32_t src_ip;
    uint16_t src_port, dst_port;

    // get udp hdr from m

    printf("this is udp rx !! \n");

    udphdr = (struct udp_hdr *)mbufpull(m, sizeof(struct udp_hdr));
    if (udphdr == NULL) {
        // free the m
        goto fail;
    }

    if (ntohs(udphdr->len) != len) {

        goto fail;
    }

    len -= sizeof(struct udp_hdr);
    
    if (len > m->len) {
        printf("%d %d", len, m->len);
        printf("fails\n");
        goto fail;
    }

    // only len in buffer saved
    mbuftrim(m, m->len - len);

    src_ip = ntohl(iphdr->src_addr);
    src_port = ntohs(udphdr->src_port);
    dst_port = ntohs(udphdr->dst_port);
    

    // TODO : throw the buffer to the socket UDP recv

    printf("------\n");
    printf("%s %d\n", "src ip", src_ip);
    printf("%s %d\n", "src port", src_port);
    printf("%s %d %d\n", "dst port", dst_port, udphdr->dst_port);

    if (1) {
        for (int i = 1; i <= len; i++)
           printf("%c", *(m->head + i));
    }
    printf("------\n");

    mbuffree(m);
    // recieve the udp packet by the socket

fail:
    return;
}
