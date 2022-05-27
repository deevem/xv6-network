#include "../udp.h"
#include "../dns.h"

void send_udp() {
    printf("%s %d\n", "eth size", sizeof(struct eth_hdr));
    printf("%s %d\n", "ip size", sizeof(struct ip_hdr));
    printf("%s %d\n", "udp size", sizeof(struct udp_hdr));

    for (int i = 1; i <= 3; i++) {
        struct mbuf* m = mbufalloc(42);
        char * payload = "test packet";
        char* tmp = mbufput(m, strlen(payload));
        for (int i =0 ;i < strlen(payload); i++)
            tmp[i] = payload[i];
        printf("%s %d\n", "running", strlen(payload));
        udp_tx(m, MAKE_IP_ADDR(192, 168, 0, 101), 38628, 12345);
    }
}

void testdns() {
    int headroom = sizeof(struct eth_hdr) + sizeof(struct ip_hdr) + sizeof(struct udp_hdr);

    struct mbuf *inputBuf = mbufalloc(headroom);  

    uint32_t dst;

    // dns server
    dst = MAKE_IP_ADDR(114, 114, 114, 114);
    
    inputBuf->len = dns_request(inputBuf->head, "bilibili.com.");
    printf("send dns start\n");

    udp_tx(inputBuf, dst, 53, 53);
    printf("send dns finish\n");
    printf("start recv \n");
}

void testnet() {

    send_udp();
    // testdns();
    printf("network test finish\n");
}


