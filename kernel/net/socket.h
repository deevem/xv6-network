#ifndef SOCKET
#define SOCKET
#include "sysheaders.h"
#include "../spinlock.h"
#include "mbuf.h"

struct socket {
    struct socket *next; // the next socket in the list
    uint8 protocol_type; // the procotol type in ip
    uint32 r_ip; // the remote IPv4 address
    uint16 lport; // the local UDP port number
    uint16 rport; // the remote UDP port number
    uint8 icmp_type;
    uint8 icmp_code;
    uint8 icmp_recvttl;
    struct spinlock lock;
    struct mbuf_queue rxq;
};

void sockinit(void);
int udp_sockalloc(struct file **f, uint32 r_ip, uint16 lport, uint16 rport);
int icmp_sockalloc(struct file ** f, uint32 r_ip, uint8 icmp_type, uint8 icmp_code);
void sockclose(struct socket * si);
int sockread(struct socket *si, uint64 addr, int n);
int sockwrite(struct socket *si, uint64 addr, int n);
void sockrecvudp(struct mbuf *m, uint32 r_ip, uint16 lport, uint16 rport);
void sockrecvicmp(struct mbuf *m, uint32 r_ip, uint8 ttl);

#endif