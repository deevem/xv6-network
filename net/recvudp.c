#include "udp.h"
#include "ethernet.h"
#include "tuntap_if.h"

#include <linux/if_packet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if.h>
#include <sys/ioctl.h>

int main() {
    tun_init();
    int t = 0;
    while(1) {
        t++;
        struct mbuf* m = mbufalloc(0);
        int n = tun_read(m->buf, MBUF_SIZE);

        printf("%s %d %d\n", "tap finished", t, n);
        m->len = n;
        ip_rx(m);
    }
    return 0;
}