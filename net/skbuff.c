#include "skbuff.h"

uint8_t *skb_push(struct sk_buff *skb, uint32_t len) {
    skb->data -= len;
    skb->len += len;
    return skb->data;
}