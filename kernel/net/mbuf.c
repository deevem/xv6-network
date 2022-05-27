#include "mbuf.h"

char* mbufpull(struct mbuf* m, unsigned int len) {
    char* tmp = m->head;
    // error when the len of mbuf less than the required length
    if (m->len < len)
        return 0;
    m->len -= len;
    m->head += len;
    return tmp;
}

char* mbufpush(struct mbuf* m, unsigned int len) {
    char *tmp = m->head - len;
    m->head -= len;
    if (m->head < m->buf) {
        // TODO : panic
        return 0;
    }
    m->len += len;
    return tmp;
}

char* mbuftrim(struct mbuf* m, unsigned int len) {

    if (len > m->len)
        return 0;
    m->len -= len;
    return m->head + m->len;
}

char* mbufput(struct mbuf* m, unsigned int len) {
    char* tmp = m->head + m->len;
    if (m->len + len > MBUF_SIZE) {
        // TODO : panic
        return 0;
    }
    m->len += len;
    return tmp;
}

struct mbuf* mbufalloc(unsigned int hdr_size) {
    struct mbuf* m;

    if (hdr_size > MBUF_SIZE)
        return 0;
    m = kalloc();

    if (m == NULL)
        return 0;

    m->next = 0;
    m->head = ( char* )m->buf + hdr_size;
    m->len = 0;
    memset(m->buf, 0, sizeof(m->buf));
    return m;
}

void mbuffree(struct mbuf *m) {
    kfree(m);
}

// ------------mbuf_queue-------------

void mbuf_queue_pushtail(struct mbuf_queue *q, struct mbuf *m) {
    q->tail->next = m;
    q->tail = m;
}

struct mbuf* mbuf_queue_pophead(struct mbuf_queue *q) {
    struct mbuf* tmp = q->head;
    printf("living %d \n", tmp->next);
    q->head = q->head->next;
    return tmp;
}

int mbuf_queue_empty(struct mbuf_queue *q) {
    if (q->head == NULL) 
        return 1;
    return 0;
}

void mbuf_queue_init(struct mbuf_queue *q) {
    q->head = NULL;
}