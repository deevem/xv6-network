#ifndef IP
#define IP

#include "ethernet.h"
#include "sysheaders.h"
#include "mbuf.h"

struct ip_hdr {
=======
    uint8_t  vhl;
>>>>>>> c0223af9505fc6a6413fa5f35a49418309c456b1
    uint8_t  tos;            // type of service
    uint16_t len;
    uint16_t id;             // identification
    uint16_t frag_offset;
    uint8_t  ttl;            // time to live
    uint8_t  protocol;
    uint16_t checksum;
    uint32_t src_addr;       // source address
    uint32_t dst_addr;       // destination ip address
};


#define IP_HDR_LEN sizeof(struct ip_hdr)

// values of ip_hdr.protocol

#define IPPROTO_ICMP 1  // Control message protocol
#define IPPROTO_TCP 6   // Transmission control protocol
#define IPPROTO_UDP 17  // User datagram protocol


void ip_rx(struct mbuf *m);
void ip_tx(struct mbuf *m, uint8_t protocol, uint32_t dst_ip);

int checksum(void *addr, int count){
    //compute sum for count bytes starting from addr
    uint16_t ptr;
    register uint32_t sum = 0;
    while(count > 1){
        sum += *ptr++;
        count -= 2;
    }
    if(count > 0){
        sum += *(uint8_t*) ptr;
    }
    while(sum >> 16)
        sum = (sum & 0xffff) + (sum >> 16)

}

#endif