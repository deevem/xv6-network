#include "icmp.h"
#include "ip.h"
#include "utils.h"
#include "socket.h"

void icmp_incoming(struct mbuf* m) {

    struct icmp_hdr* icmphdr = (struct icmp_hdr*)mbufpull(m, sizeof(struct icmp_hdr));

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
    icmphdr->cksum = checksum((unsigned char*)icmphdr, tmp);

    uint32_t x = iphdr->src_addr;
    iphdr->src_addr = iphdr->dst_addr;
    iphdr->dst_addr = x;

    iphdr->protocol = IPPROTO_ICMP;

    ip_tx_ready(m);

    mbuffree(m);
}

void icmp_tx(struct mbuf *m, uint32 dst_ip, uint8 type, uint8 code)
{
    struct icmp_hdr * icmphdr = (struct icmp_hdr*) mbufpush(m, sizeof(struct icmp_hdr));
    icmphdr->type = type;
    icmphdr->code = code;
    icmphdr->cksum = checksum((unsigned char*)icmphdr, m->len);

    ip_tx(m, IPPROTO_ICMP, dst_ip);
}

void icmp_rx(struct mbuf *m, uint16 len, struct ip_hdr *iphdr)
{
    struct icmp_hdr * icmphdr = (struct icmp_hdr*) mbufpush(m, sizeof(struct icmp_hdr));
    if (!icmphdr)
        goto fail;
    
    len -= sizeof(*icmphdr);
    if (len > m->len)
        goto fail;
    
    if(checksum((unsigned char*)icmphdr, m->len))
        panic("wrong checksum");
    
    uint8 ttl = iphdr->ttl;
    uint32 sip = ntohl(iphdr->src_addr);
    sockrecvicmp(m, sip, ttl);
fail:
    mbuffree(m);
}