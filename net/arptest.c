#include "arp.h"
#include "tuntap_if.h"

int main() {
    tap_init();
    sleep(2);
    int headroom = sizeof(struct eth_hdr) + sizeof(struct arp_hdr);
    struct mbuf* m = mbufalloc(headroom);
    arp_tx(ARP_OP_REQUEST, broadcast_mac, MAKE_IP_ADDR(101, 5, 172, 230));

    int t = 0;
    while(1) {
        t++;
        struct mbuf* m = mbufalloc(0);
        int n = tun_read(m->buf, MBUF_SIZE);

        printf("%s %d %d\n", "tun finished", t, n);
        m->len = n;
        eth_rx(m);
    }

    return 0;
}