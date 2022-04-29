#include "udp.h"
#include "tuntap_if.h"

int main() {
    tun_init();
    printf("%s", "tun finished\n");
    printf("%s %d\n", "eth size", sizeof(struct eth_hdr));
    printf("%s %d\n", "ip size", sizeof(struct ip_hdr));
    printf("%s %d\n", "udp size", sizeof(struct udp_hdr));
    int t = 0;
    while(1) {
        sleep(2);
        t++;
        printf("%s %d\n", "test ", t);
        struct mbuf* m = mbufalloc(42);
        char * payload = "test";
        char* tmp = mbufput(m, strlen(payload));
        for (int i =0 ;i < strlen(payload); i++)
            tmp[i] = payload[i];
        printf("%s %d\n", "running", strlen(payload));
        udp_tx(m, MAKE_IP_ADDR(10,1,1,2), 38628, 12345);
    }
    return 0;
}