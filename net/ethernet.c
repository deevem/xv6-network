#include "ethernet.h"
#include "tuntap_if.h"
#include "ip.h"
#include "arp.h"

#include <linux/if_packet.h>
#include <sys/socket.h>
#include <net/if.h>
#include <linux/if.h>
#include <sys/ioctl.h>

void eth_tx(struct mbuf *m, uint16_t ethType) {

    struct eth_hdr *ethhdr;

    ethhdr = (struct eth_hdr*)mbufpush(m, sizeof(struct eth_hdr));

    // broadcast
    for (int i = 0; i < ETHADDR_LEN; i++) {
        ethhdr->srchost[i] = local_mac[i];
        ethhdr->dsthost[i] = broadcast_mac[i];
    }
    ethhdr->type = htons(ethType);

    printf("%s \n", "ethernet tx ready");
    printf("%s %d\n", "len of buf", m->len);
    printf("%s %d\n", "head", m->head - m->buf);

    struct ip_hdr *iphdr;
    iphdr = (struct ip_hdr*)(m->buf + 14);
    printf("%s %d\n", "ttl test", iphdr->ttl);
    
    int n = tun_write(m->buf, m->len);

	// struct sockaddr_in sin, din;
    // int one = 1;
    // const int *val = &one;
    
    // int sd = socket(AF_INET, SOCK_RAW, IPPROTO_RAW);
	// if (sd < 0) {
	// 	perror("socket() error");
	// 	exit(-1);
	// }
	
	// if (setsockopt(sd, IPPROTO_IP, IP_HDRINCL, val, sizeof(int))) {
	// 	perror("setsockopt() error");
	// 	exit(-1);
	// }
	// else
	// 	printf("setsockopt() is OK.\n");

	// din.sin_family = AF_INET;
	// din.sin_addr.s_addr = inet_addr("10.1.1.3");
	// din.sin_port = htons(12345);

    // printf("%d %d %d %d\n", sd, m->len, sizeof(din), (struct sockaddr *)&din);

    // if (sendto(sd, m->buf +14, m->len, 0, (struct sockaddr *)&din, sizeof(din)) < 0) {
    //     perror("sendto() error");
    //     exit(-1);
    // }
    // else
    //     printf("sendto() is OK.\n");

    // close(sd);
    mbuffree(m);
    printf("%s %d\n", "finished tun write", n);
}

void eth_rx(struct mbuf* m) {
    struct eth_hdr *ethhdr;
    uint16_t ethtype;
    ethhdr = (struct eth_hdr*) mbufpull(m, sizeof(struct eth_hdr));
    printf("running here\n");

    if (ethhdr == NULL) {
        printf("fail in eth \n");
        return;
    }
    printf("%s %d\n", "ethtype", ethtype);
    ethtype = ntohs(ethhdr->type);

    if (ethtype == ETHTYPE_IP)
        ip_rx(m);
    else if (ethtype == ETHTYPE_ARP)
        arp_rx(m);
    else 
        mbuffree(m);
}