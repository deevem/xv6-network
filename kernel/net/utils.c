#include "utils.h"
#include "ip.h"

uint16_t checksum(const unsigned char* addr, int len) {

    int nleft = len;
    const uint16_t *w = (const uint16_t*) addr;
    uint32_t sum = 0;
    uint16_t answer = 0;

    while(nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *)(&answer) = *(const unsigned char *)w;
        sum += answer;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);

    answer = ~sum; 

    return answer;

}

uint16_t tcp_checksum(struct mbuf* m, uint32_t saddr, uint32_t daddr){
    int len = m->len;
    

    int nleft = len;
    const uint16_t *w = (const uint16_t*) m;
    uint32_t sum = 0;
    uint16_t answer = 0;

    sum += saddr;
    sum += daddr;
    sum += htons(IPPROTO_TCP);
    sum += htons(m->len);

    while(nleft > 1) {
        sum += *w++;
        nleft -= 2;
    }

    if (nleft == 1) {
        *(unsigned char *)(&answer) = *(const unsigned char *)w;
        sum += answer;
    }

    sum = (sum & 0xffff) + (sum >> 16);
    sum += (sum >> 16);

    answer = ~sum; 

    return answer;
}