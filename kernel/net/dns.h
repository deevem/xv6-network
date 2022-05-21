#ifndef DNS
#define DNS
#include "sysheaders.h"

struct dns_hdr {
  uint16_t id;  // request ID

  uint8_t rd: 1;  
  uint8_t tc: 1;  
  uint8_t aa: 1;  
  uint8_t opcode: 4; 
  uint8_t qr: 1;  
  uint8_t rcode: 4; 
  uint8_t cd: 1;  
  uint8_t ad: 1;  
  uint8_t z:  1;  
  uint8_t ra: 1; 
  
  uint16_t qdcount; 
  uint16_t ancount; 
  uint16_t nscount; 
  uint16_t arcount; 
} __attribute__((packed));

struct dns_question {
  uint16_t qtype;
  uint16_t qclass;
} __attribute__((packed));
  
#define ARECORD (0x0001)
#define QCLASS  (0x0001)

struct dns_data {
  uint16_t type;
  uint16_t dclass;
  uint32_t ttl;
  uint16_t len;
} __attribute__((packed));


int dns_request(char* buf, const char* host);
int dns_response(char* buf, int recv_len);

#endif