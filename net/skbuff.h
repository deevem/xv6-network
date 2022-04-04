// header file of sk buff

// skbuff = socket buff = skb

#ifndef SKBUFF_H
#define SKBUFF_h

#include "sysheaders.h"
#include "list.h"

// header of sk buff
struct sk_buff_hdr {
    struct list_head *head;
    uint32_t qlen; // the len of buffer queue 
};

struct sk_buff {
    struct list_head list;
    uint16_t protocol;          // type of protocol
    uint32_t len;               // the length of the block of data in the buffer
    uint32_t data_len;           // the lenght of only the data in fragments
    
    uint8_t *head;
    uint8_t *data;
    uint8_t *tail;
    uint8_t *end;
    uint8_t *payload;
};

static inline uint32_t skb_queue_len(const struct sk_buff_hdr *hdr) {
    return hdr->qlen;
}

static inline void skb_queue_init(struct sk_buff_hdr *hdr) {
    list_init(hdr->head);
    hdr->qlen = 0;
}

static inline void skb_queue_add(struct sk_buff_hdr *hdr, struct sk_buff *newitem, struct sk_buff *next) {
    list_add_tail(&newitem->list, &next->list);
    hdr->qlen += 1;
}

uint8_t *skb_push(struct sk_buff *skb, uint32_t len);


#endif