#include "dns.h"
#include "udp.h"
#include "mbuf.h"
#include "tuntap_if.h"

int main() {
    
    tun_init();

    int headroom = sizeof(struct ip_hdr) + sizeof(struct udp_hdr);

    struct mbuf *inputBuf = mbufalloc(headroom);    
    struct mbuf *outputBuf = mbufalloc(0);    

    uint32_t dst;

    // dns server
    dst = MAKE_IP_ADDR(101,5,172,230);
    
    inputBuf->len = dns_request(inputBuf->head, "bilibili.com.");
    sleep(5);
    udp_tx(inputBuf, dst, 38628, 12345);
    printf("start recv \n");

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