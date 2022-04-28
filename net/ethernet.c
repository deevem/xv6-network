#include "ethernet.h"
#include "tuntap_if.h"

void eth_tx(struct mbuf *m, uint16_t ethType) {

    struct eth_hdr *ethhdr;

    ethhdr = (struct eth_hdr*)mbufpush(ethhdr, sizeof(struct eth_hdr));

    // broadcast
    for (int i = 0; i < ETHADDR_LEN; i++) {
        ethhdr->srchost[i] = local_mac[i];
        ethhdr->dsthost[i] = broadcast_mac[i];
    }

    tun_write(m->buf, MBUF_SIZE);
}
