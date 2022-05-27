#include "ethernet.h"
#include "ip.h"
#include "arp.h"

// 52:54:00:12:34:56
static uint8_t local_mac[ETHADDR_LEN] = { 0x52, 0x54, 0x00, 0x12, 0x34, 0x56 };
// static uint8_t local_mac[ETHADDR_LEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
static uint8_t broadcast_mac[ETHADDR_LEN] = { 0xFF, 0XFF, 0XFF, 0XFF, 0XFF, 0XFF };

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
    
    int n = e1000_transmit(m);

    mbuffree(m);
    printf("%s %d\n", "finished net write", n);
}

void eth_rx(struct mbuf* m) {
    struct eth_hdr *ethhdr;
    uint16_t ethtype;

    ethhdr = (struct eth_hdr*) mbufpull(m, sizeof(struct eth_hdr));
    printf("receive eth packet\n");

    if (ethhdr == NULL) {
        printf("fail in eth \n");
        return;
    }
    ethtype = ntohs(ethhdr->type);
    printf("%s %d\n", "ethtype", ethtype);

    if (ethtype == ETHTYPE_IP)
        ip_rx(m);
    else if (ethtype == ETHTYPE_ARP)
        arp_rx(m);
    else 
        mbuffree(m);

    printf("fininsh ethernet\n");
}
