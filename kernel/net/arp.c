#include "arp.h"

static uint8_t local_mac[ETHADDR_LEN] = { 0x52, 0x54, 0x00, 0x12, 0x34, 0x56 };

void arp_tx(uint16_t op, uint8_t desmac[ETHADDR_LEN], uint32_t tip){
    struct arp_hdr* hdr;
    struct mbuf* m;

    m = mbufalloc(sizeof(struct eth_hdr) + sizeof(struct arp_hdr));
    hdr = (struct arp_hdr*)mbufpush(m, sizeof(struct arp_hdr));
    
    uint32_t local_ip = MAKE_IP_ADDR(10,1,1,5);

    hdr->ar_hrd = htons(ARP_HRD_ETHER);
    hdr->ar_pro = htons(ETHTYPE_IP);
    hdr->ar_hln = ETHADDR_LEN;
    hdr->ar_pln = sizeof(uint32_t);
    hdr->ar_op = htons(op);

    memmove(hdr->arp_sha,local_mac,ETHADDR_LEN);
    hdr->arp_sip = htonl(local_ip);
    
    memmove(hdr->arp_tha,desmac,ETHADDR_LEN);
    hdr->arp_tip = htonl(tip);

    eth_tx(m,ETHTYPE_ARP);
}

void arp_rx(struct mbuf* mbuffer) {

    struct arp_hdr* hdr;
    uint8_t smac[ETHADDR_LEN];
    
    uint32_t local_ip = MAKE_IP_ADDR(10,1,1,5);
    hdr = (struct arp_hdr*)mbufpull(mbuffer,sizeof(*hdr));


    if(!hdr)
        goto fin;
    
    if (ntohs(hdr->ar_hrd) != ARP_HRD_ETHER || 
        ntohs(hdr->ar_pro) != ETHTYPE_IP || 
        hdr->ar_hln != ETHADDR_LEN ||
        hdr->ar_pln != sizeof(uint32_t))
        goto fin;

    if (ntohl(hdr->arp_tip) != local_ip || ntohs(hdr->ar_op) != ARP_OP_REQUEST)
        goto fin;

    memmove(smac,hdr->arp_sha,ETHADDR_LEN);

    arp_tx(ARP_OP_REPLY,smac, ntohl(hdr->arp_sip));
fin:
    mbuffree(mbuffer);
}