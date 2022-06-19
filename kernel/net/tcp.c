#include "tcp.h"
#include "utils.h"

void tcp_tx(struct tcp_sock *ts, struct tcp_hdr *tcphdr, struct mbuf *m, uint16_t seq) {
    tcphdr->data_offset = TCP_DOFFSET;
    tcphdr->src_port = htons(ts->src_port);
    tcphdr->dst_port = htons(ts->dst_port);
    tcphdr->seq = htonl(seq);
    tcphdr->ack_seq = htonl(ts->tcb.recv_next);
    tcphdr->reserved = htons(0);
    tcphdr->window = htons(ts->tcb.recv_window);
    tcphdr->checksum = checksum(m, m->len);
    tcphdr->urg = htons(0);
}

void tcp_send_syn(struct tcp_sock *ts) {
    if (ts->state == TCP_CLOSE)
        return;
    
    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 1;

    tcp_tx(ts, tcphdr, m, ts->tcb.send_next);
}