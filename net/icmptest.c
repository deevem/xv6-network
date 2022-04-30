#include "ethernet.h"
#include "tuntap_if.h"

int main() {
    tun_init();
    int t = 0;
    while(1) {
        t++;
        struct mbuf* m = mbufalloc(0);
        int n = tun_read(m->buf, MBUF_SIZE);

        printf("%s %d %d\n", "tun finished", t, n);
        m->len = n;
        ip_rx(m);
    }
    return 0;
}