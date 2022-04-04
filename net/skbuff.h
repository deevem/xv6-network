// header file of sk buff

// skbuff = socket buff = skb

#ifndef SKBUFF_H
#define SKBUFF_h

#include "sysheaders.h"

// header of sk buff
struct sk_buff_hdr {
    sk_buff_hdr* next;
    sk_buff_hdr* prev;

    uint32_t qlen;
};

struct sk_buff {
    
};

#endif