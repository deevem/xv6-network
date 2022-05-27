#ifndef DNS
#define DNS
#include "../kernel/types.h"
#include "../kernel/param.h"
#include "../kernel/riscv.h"
static inline uint16 bswaps(uint16 val)
{
  return (((val & 0x00ffU) << 8) |
          ((val & 0xff00U) >> 8));
}

static inline uint32 bswapl(uint32 val)
{
  return (((val & 0x000000ffUL) << 24) |
          ((val & 0x0000ff00UL) << 8) |
          ((val & 0x00ff0000UL) >> 8) |
          ((val & 0xff000000UL) >> 24));
}

#define ntohs bswaps
#define ntohl bswapl
#define htons bswaps
#define htonl bswapl

#define NULL 0


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


int dns_request(uint8* buf, const char* host);
int dns_response(uint8* buf, int recv_len);

uint32 dns(char* ss);

#endif