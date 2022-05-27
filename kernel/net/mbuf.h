#ifndef MBUF
#define MBUF
#include "sysheaders.h"

#define MBUF_SIZE 4096
#define MBUF_DEFAULT_HEADROOM  128
struct mbuf {
    struct mbuf* next;
    char*        head;
    uint8_t      len;
    char         buf[MBUF_SIZE];
};

// add len to the header size
char *mbufpull(struct mbuf* m, unsigned int len);
// pop len of the header size
char *mbufpush(struct mbuf* m, unsigned int len);


// start: [header][buffer][tail] 
// end  : [header][buffer   ][tail]
//                       - return value 
char *mbufput(struct mbuf* m, unsigned int len);

// start: [header][buffer   ][tail] 
// end  : [header][buffer][tail]
//                       - return value 
char *mbuftrim(struct mbuf* m, unsigned int len);


// memory operation

struct mbuf* mbufalloc(unsigned int hdr_size);
void mbuffree(struct mbuf* m);

// queue of mbuf
struct mbuf_queue {
    struct mbuf* head;
    struct mbuf* tail;
};

void mbuf_queue_pushtail(struct mbuf_queue *q, struct mbuf *m);
struct mbuf* mbuf_queue_pophead(struct mbuf_queue *q);
int mbuf_queue_empty(struct mbuf_queue *q);
void mbuf_queue_init(struct mbuf_queue *q);

#endif