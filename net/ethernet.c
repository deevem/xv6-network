#include "ethernet.h"
#include "tuntap_if.h"
#include "ip.h"

void eth_tx(struct mbuf *m, uint16_t ethType) {

    struct eth_hdr *ethhdr;

    ethhdr = (struct eth_hdr*)mbufpush(m, sizeof(struct eth_hdr));

    // broadcast
    for (int i = 0; i < ETHADDR_LEN; i++) {
        ethhdr->srchost[i] = local_mac[i];
        ethhdr->dsthost[i] = broadcast_mac[i];
    }
    ethhdr->type = htons(ethType);

    printf("%s \n", "ethernet tx ready");
    printf("%s %d\n", "len of buf", m->len);
    printf("%s %d\n", "head", m->head - m->buf);

    struct ip_hdr *iphdr;
    iphdr = (struct ip_hdr*)(m->buf + 14);
    printf("%s %d\n", "ttl test", iphdr->ttl);
    
    int n = tun_write(m->buf, MBUF_SIZE);

    mbuffree(m);

    printf("%s %d\n", "finished tun write", n);
}
