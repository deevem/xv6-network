#include "arp.h"
#include "tuntap_if.h"

int main() {
    tap_init();
    int t = 0;
    while(1) {
        sleep(2);
        t++;
        printf("%s %d\n", "ARP test ", t);

        int headroom = sizeof(struct eth_hdr) + sizeof(struct arp_hdr);

        struct mbuf* m = mbufalloc(headroom);
        arp_tx(ARP_OP_REQUEST, broadcast_mac, MAKE_IP_ADDR(101, 5, 172, 230));
    }
    return 0;
}