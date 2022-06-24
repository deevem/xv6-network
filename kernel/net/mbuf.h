#ifndef MBUF
#define MBUF
#include "sysheaders.h"
#include "list.h"
#define MBUF_SIZE 256
#define MBUF_DEFAULT_HEADROOM  128
struct mbuf {
    struct mbuf* next;
    char*        head;
    uint8_t      len;
    char         buf[MBUF_SIZE];

    // for tcp items
    int refcnt;
    uint32_t start_seq;
    uint32_t end_seq;
    struct list_head list;
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

struct tcp_mbuf_queue {
    struct list_head head;
    uint32 len;
};

static inline uint32_t tcp_mbuf_queue_len(const struct tcp_mbuf_queue *q) {
    return q->len;
}

static inline void tcp_mbuf_queue_init(struct tcp_mbuf_queue *q) {
    list_init(&q->head);
    q->len = 0;
}

static inline void tcp_mbuf_queue_add(struct tcp_mbuf_queue* q, struct mbuf* item, struct mbuf* next) {
    list_add_tail(&item->list, &next->list);
    q->len++;
}

static inline void tcp_mbuf_enqueue(struct tcp_mbuf_queue* q, struct mbuf* item) {
    list_add_tail(&item->list, &q->head);
    q->len++;
}

static inline struct mbuf* tcp_mbuf_dequeue(struct tcp_mbuf_queue* q){
    struct mbuf *m = list_first_entry(&q->head, struct mbuf, list);
    list_del(&m->list);
    q->len--;
    return m;
}

static inline int tcp_mbuf_queue_empty(const struct tcp_mbuf_queue *q){
    return tcp_mbuf_queue_len(q) < 1;
}

static inline struct mbuf* tcp_mbuf_queue_peek(struct tcp_mbuf_queue *q){
    if (tcp_mbuf_queue_empty(q))
        return NULL;
    return list_first_entry(&q->head, struct mbuf, list);
}
#endif