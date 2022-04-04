#include "netdev.h"
#include "tuntap_if.h"
#include "ethernet.h"

int netdev_tx(struct sk_buff* skb, uint8_t* dst, uint16_t ethertype) {
    struct netdev *dev;
    struct eth_hdr *hdr;
    int ret = 0;

    skb_push(skb, ETH_HDR_LEN);

    ret = tun_write((char *) skb->data, skb->len);
    return ret;
}