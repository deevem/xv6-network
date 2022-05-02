#ifndef ARP
#define ARP

#include "mbuf.h"
#include "ethernet.h"

#define ARP_HRD_ETHER 1

struct arp_hdr{
    uint16_t ar_hrd;//hardware type
    uint16_t ar_pro;//protocol type
    uint8_t ar_hln;//hardware address length
    uint8_t ar_pln;//protocol address length
    uint16_t ar_op;//request / response
    uint8_t arp_sha[ETHADDR_LEN];//sender mac address
    uint32_t arp_sip;//sender ip address
    uint8_t arp_tha[ETHADDR_LEN];//receiver mac address
    uint32_t arp_tip;//receiver ip address
} __attribute__((packed));

enum {
    ARP_OP_REQUEST = 1,
    ARP_OP_REPLY = 2,
};

void arp_tx(uint16_t op, uint8_t desmac[ETHADDR_LEN], uint32_t tip);
void arp_rx(struct mbuf* mbuffer);

#endif