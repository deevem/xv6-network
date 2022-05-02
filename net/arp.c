#include "arp.h"

static void arp_tx(uint16_t op, uint8_t desmac[ETHADDR_LEN], uint32_t tip){
    struct arp_hdr* hdr;
    struct mbuf* m;

    m = mbufalloc(MBUF_SIZE);
    
    uint32_t local_ip = MAKE_IP_ADDR(10,1,1,5);

    hdr = mbufput(m,sizeof(*hdr));
    hdr->ar_hrd = ARP_HRD_ETHER;
    hdr->ar_pro = ETHTYPE_IP;
    hdr->ar_hln = ETHADDR_LEN;
    hdr->ar_pln = sizeof(uint32_t);
    hdr->ar_op = htons(op);

    memmove(hdr->arp_sha,local_mac,ETHADDR_LEN);
    hdr->arp_sip = local_ip;
    memmove(hdr->arp_tha,desmac,ETHADDR_LEN);
    hdr->arp_tip = tip;

    eth_tx(m,ETHTYPE_ARP);
}

static void arp_rx(struct mbuf* mbuffer){

    struct arp_hdr* hdr;
    uint8_t smac[ETHADDR_LEN];
    uint32_t sip,tip;
    
    hdr = mbufpull(mbuffer,sizeof(*hdr));
    if(!hdr)
        goto fin;
    
    if (ntohs(hdr->ar_hrd) != ARP_HRD_ETHER || 
        ntohs(hdr->ar_pro) != ETHTYPE_IP || 
        hdr->ar_hln != ETHADDR_LEN ||
        hdr->ar_pln != sizeof(uint32_t))
        goto fin;

    if (ntohs(hdr->arp_tip != local_ip) || ntohs(hdr->ar_op != ARP_OP_REQUEST))
        goto fin;
    
    memmove(smac,hdr->arp_sha,ETHADDR_LEN);
    arp_tx(ARP_OP_REPLY,smac,hdr->arp_sip);
fin:
    mbuffree(mbuffer);
}