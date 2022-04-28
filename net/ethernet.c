#include "ethernet.h"

static void eth_tx(struct mbuf *m, uint16_t ethType) {

    struct eth_hdr *ethhdr;

    ethhdr = (struct eth_hdr*)mbufpush(ethhdr, sizeof(struct eth_hdr));
}
