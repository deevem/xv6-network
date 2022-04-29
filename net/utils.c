#include "utils.h"

uint8_t checksum(const unsigned char* addr, int len) {

    int nleft = len;
    const uint8_t *w = (const uint8_t*) addr;
    uint16_t sum = 0;
    uint8_t answer = 0;

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