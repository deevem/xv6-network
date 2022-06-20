#include "tcp.h"
#include "list.h"

int tcp_listen(struct tcp_sock* tcpsock, struct tcp_hdr* tcphdr, struct ip_hdr *iphdr) {
    if (tcphdr->syn != 1 || tcphdr->ack != 0)
        return -1;
    
    tcpsock->dst_addr = iphdr->src_addr;
    tcpsock->dst_port = iphdr->src_addr;
    tcpsock->src_addr = iphdr->dst_addr;

    tcpsock->tcb.irs = tcphdr->seq;
    tcpsock->tcb.iss = alloc_new_iss();
    tcpsock->tcb.recv_next = tcphdr->seq + 1;
    tcpsock->tcb.send_next = tcpsock->tcb.iss + 1;
    tcpsock->tcb.send_unack = tcpsock->tcb.iss;

    tcp_set_state(tcpsock, TCP_SYN_RECEIVED);
    tcp_send_synack(tcpsock);

    return 0;
}

int tcp_synsent(struct tcp_sock* tcpsock, struct tcp_hdr* tcphdr) {
    if (tcpsock->state != TCP_SYN_SENT)
        return -1;
    if (tcphdr->ack != 1 || tcphdr->syn != 1)
        return -1;
    
    tcp_set_state(tcpsock, TCP_ESTABLISHED);
    tcp_send_ack(tcpsock);
    return 0;
}

int tcp_synrecv(struct tcp_sock* tcpsock, struct tcp_hdr* tcphdr) {
    
    if (tcphdr->ack != 1 || tcphdr->syn != 0)
        return -1;
    
    tcp_set_state(tcpsock, TCP_ESTABLISHED);
    return 0;
}

void tcp_update_window(struct tcp_sock *tcpsock, struct tcp_hdr *tcphdr) {
    if (tcpsock->tcb.send_unack <= tcphdr->ack_seq && tcpsock->send_next >= tcphdr->ack_seq)
        if (tcpsock->tcb.send_win1 < tcphdr->seq || (tcpsock->tcb.send_win1 == tcphdr->seq && tcpsock->tcb.send_win2 <= tcphdr->ack_seq)) {
            tcpsock->tcb.send_window = tcphdr->window;
            tcpsock->tcb.send_win1 = tcphdr->seq;
            tcpsock->tcb.send_win2 = tcphdr->ack_seq;
        }
}

int tcp_input_state(struct tcp_sock *tcpsock, struct tcp_hdr *tcphdr, struct ip_hdr *iphdr, struct mbuf* m) {
    switch (tcpsock->state) {
        case TCP_CLOSE:
            return tcp_closed(tcpsock, tcphdr, m);
        case TCP_LISTEN:
            return tcp_listen(tcpsock, tcphdr, iphdr);
        case TCP_SYN_SENT:
            return tcp_synsent(tcpsock, tcphdr);
        case TCP_SYN_RECEIVED:
            return tcp_synrecv(tcpsock, tcphdr);
    }
}