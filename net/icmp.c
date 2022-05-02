#include "icmp.h"
#include "ip.h"
#include "utils.h"

void icmp_incoming(struct mbuf* m) {

    struct icmp_hdr* icmphdr = (struct icmp_hdr*)mbufpull(m, sizeof(struct icmp_hdr));
    struct ip_hdr *iphdr = (struct ip_hdr *)((char*)icmphdr - sizeof(struct ip_hdr));

    switch (icmphdr->type) {
    case ICMP_ECHO:
        icmp_reply(m);
        return;
    case ICMP_DST_UNREACHABLE:
        printf("haha ICMP_DST_UNREACHABLE GG");
    default:
        printf("haha GG");
        break;
    }

    mbuffree(m);
    return;
}

void icmp_reply(struct mbuf* m) {
    printf("running\n");
    struct icmp_hdr* icmphdr = (struct icmp_hdr*)mbufpush(m, sizeof(struct icmp_hdr));
    int tmp = m->len;
    struct ip_hdr *iphdr = (struct ip_hdr *)((char*)icmphdr - sizeof(struct ip_hdr));
    printf("pos %d %d \n", m->head - m->buf, m->len);

    icmphdr->type = ICMP_REPLY;
    icmphdr->cksum = 0;
    icmphdr->cksum = checksum(icmphdr, tmp);

    uint32_t x = iphdr->src_addr;
    iphdr->src_addr = iphdr->dst_addr;
    iphdr->dst_addr = x;

    iphdr->protocol = IPPROTO_ICMP;

    ip_tx_ready(m);

    mbuffree(m);
}