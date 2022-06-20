#include "tcp.c"

struct tcp_sock *tcp_listen_subsock(struct tcp_sock* tcpsock, struct tcp_hdr* tcphdr, struct ip_hdr *iphdr) {
    struct tcp_sock *new_tcpsock = tcp_sock_alloc();
    tcp_set_state(new_tcpsock, TCP_SYN_RECEIVED);
    new_tcpsock->src_addr = ntohl(iphdr->dst_addr);
    new_tcpsock->dst_addr = ntohl(iphdr->src_addr);
    new_tcpsock->src_port = tcphdr->dst_port;
    new_tcpsock->src_addr = tcphdr->src_port;
    new_tcpsock->parent = tcpsock;

    list_add(&new_tcpsock->list, &tcpsock->listen_queue);

    return new_tcpsock;
}

int tcp_listen(struct tcp_sock *tcpsock, struct tcp_hdr* tcphdr, struct ip_hdr *iphdr, struct mbuf *m) {
    struct tcp_sock *new_tcpsock;

    if (tcphdr->ack == 1) {
        tcp_send_reset(tcpsock);
        goto fail;
    }

    if (tcphdr->syn != 0)
        goto fail;

    new_tcpsock = tcp_listen_subsock(tcpsock, tcphdr, iphdr);
    
    if (new_tcpsock == NULL)
        goto fail;
    
    new_tcpsock->tcb.irs = tcphdr->seq;
    new_tcpsock->tcb.iss = alloc_new_iss();
    new_tcpsock->tcb.recv_next = tcphdr->seq + 1;
    new_tcpsock->tcb.send_next = new_tcpsock->tcb.iss + 1;
    new_tcpsock->tcb.send_unack = new_tcpsock->tcb.iss;

    tcp_send_synack(tcpsock);
fail:
    mbuffree(m);
    return 0;
}

int tcp_synsent(struct tcp_sock *tcpsock, struct tcp_hdr* tcphdr, struct ip_hdr *iphdr, struct mbuf *m) {
    
    if (tcphdr->syn == 1 && tcphdr->ack == 1) {
        tcpsock->tcb.irs = tcphdr->seq;
        tcpsock->tcb.recv_next = tcphdr->seq + 1;
        tcpsock->tcb.send_window = tcphdr->window;
        tcpsock->tcb.send_win1 = tcphdr->seq;
        tcpsock->tcb.send_win2 = tcphdr->ack_seq;
        tcp_set_state(tcpsock, TCP_ESTABLISHED);
        tcp_send_ack(tcpsock);
        wakeup(&tcpsock->wait_connect);
    }
    
    mbuffree(m);
    return 0;
}

int tcp_synrecv(struct tcp_sock *tcpsock) {
    if (!tcpsock->parent || tcpsock->parent->state != TCP_LISTEN)
        return -1;
    if (tcpsock->parent->accept_backlog >= tcpsock->parent->backlog)
        return -1;
    list_del(&tcpsock->list);
    list_add(&tcpsock->list, &tcpsock->parent->accept_queue);
    tcpsock->parent->accept_backlog++;
    wakeup(&tcpsock->parent->wait_accept);
    return 0;
}

int tcp_closed(struct mbuf *m) {
    mbuffree(m);
    return 0;
}