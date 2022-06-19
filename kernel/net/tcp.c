#include "tcp.h"
#include "utils.h"
#include "proc.h"

void tcp_tx(struct tcp_sock *tcpsock, struct tcp_hdr *tcphdr, struct mbuf *m, uint16_t seq) {
    tcphdr->data_offset = TCP_DOFFSET;
    tcphdr->src_port = htons(tcpsock->src_port);
    tcphdr->dst_port = htons(tcpsock->dst_port);
    tcphdr->seq = htonl(seq);
    tcphdr->ack_seq = htonl(tcpsock->tcb.recv_next);
    tcphdr->reserved = htons(0);
    tcphdr->window = htons(tcpsock->tcb.recv_window);
    tcphdr->checksum = checksum(m, m->len);
    tcphdr->urg = htons(0);

    ip_tx(m, IPPROTO_TCP, tcpsock->dst_addr);
}

void tcp_send_syn(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;
    
    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 1;

    tcp_tx(tcpsock, tcphdr, m, tcpsock->tcb.iss);
}

void tcp_send_synack(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;

    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 1;
    tcphdr->ack = 1;

    tcp_tx(tcpsock, tcphdr, m, tcpsock->tcb.iss);
}

void tcp_send_ack(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;

    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 0;
    tcphdr->ack = 1;
    tcp_tx(tcpsock, tcphdr, m, tcpsock->tcb.send_next);
}

int tcp_send(struct tcp_sock *tcpsock, uint64_t *buffer, int len) {
    int rest_len = len;
    
    while (rest_len > 0) {
        int packet_len = min(TCP_DEFALUT_MSS, rest_len);
        struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
        if (m == NULL)
            return len - rest_len;
        mbufput(m, packet_len);
        copyin(myproc()->pagetable, m->head, buffer, packet_len);
        buffer += packet_len;

        struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
        tcphdr->ack = 1;
        if (rest_len == 0)
            tcphdr->psh = 1;
        
        tcp_tx(tcpsock, tcphdr, m, tcpsock->tcb.send_next);
        tcpsock->tcb.send_next += packet_len;
    }
}