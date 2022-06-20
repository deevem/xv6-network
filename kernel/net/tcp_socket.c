#include "tcp.h"
#include "ip.h"


int tcp_connect(struct file *f, uint16_t dst_addr, uint16_t dst_port, int addrlen, int src_port) {
    struct tcp_sock *tcpsock = f->tcpsock;
    acquire(&tcpsock->spinlk);
    if (tcpsock->state != TCP_CLOSE) {
        release(&tcpsock->spinlk);
        return -1;
    }

    tcpsock->src_port = src_port;
    tcpsock->dst_port = dst_port;
    tcpsock->src_addr = local_ip;
    tcpsock->dst_addr = dst_addr;

    tcpsock->state = TCP_SYN_SENT;
    tcpsock->tcb.iss = 0; // TODO random iss
    tcpsock->tcb.send_unack = tcpsock->tcb.iss;
    tcpsock->tcb.send_next = tcpsock->tcb.iss + 1;

    tcp_send_syn(tcpsock);

    // TODO: wait connect

    if (tcpsock->state != TCP_ESTABLISHED) {
        release(&tcpsock->spinlk);
        tcp_set_state(tcpsock, TCP_CLOSE);
        return -1;
    }

    release(&tcpsock->spinlk);    
    return 0;
}

int tcp_listen(struct file *f, int backlog) {
    struct tcp_sock *tcpsock = f->tcpsock;
    acquire(&tcpsock->spinlk);

    if ( backlog > TCP_MAX_BACKLOG || tcpsock->state != TCP_CLOSE || tcpsock->src_port == 0) {
        release(&tcpsock->spinlk);
        return -1;
    }

    tcpsock->state = TCP_LISTEN;
    tcpsock->backlog = backlog;

    release(&tcpsock->spinlk);
    return 0;
}

struct tcp_sock * tcp_accept(struct file *f) {
    struct tcp_sock *tcpsock = f->tcpsock;
    acquire(&tcpsock->spinlk);

    while (list_empty(&tcpsock->accept_queue))
        sleep(&tcpsock->wait_accept, &tcpsock->spinlk);

    struct tcp_sock *new_tcpsock = list_first_entry(&tcpsock->accept_queue, struct tcp_sock, list);
    list_del_init(&new_tcpsock->list);
    tcpsock->accept_backlog--;

    release(&tcpsock->spinlk);
    return new_tcpsock;
}

int tcp_read(struct file *f, uint64_t addr, int n) {
    int rest_len = 0;
    struct tcp_sock *tcpsock = f->tcpsock;
    
    if (tcpsock == NULL)
        return -1;
    else if (tcpsock->state == TCP_CLOSE)
        return -1;
    
    acquire(&tcpsock->spinlk);
    rest_len = tcp_receive(tcpsock, addr, n);
    release(&tcpsock->spinlk);

    return rest_len;
}

int tcp_write(struct file *f, uint64_t buffer, int len) {

    struct tcp_sock *tcpsock = f->tcpsock;
    if (tcpsock == NULL)
        return -1;

    if (tcpsock->state != TCP_ESTABLISHED && tcpsock->state != TCP_CLOSE_WAIT)
        return -1;

    acquire(&tcpsock->spinlk);
    int res = tcp_send(tcpsock, buffer, len);
    release(&tcpsock->spinlk);
    return res;
}

int tcp_close(struct file *f) {
    struct tcp_sock *tcpsock = f->tcpsock;

    acquire(&tcpsock->spinlk);
    
    switch (tcpsock->state) {
        case TCP_CLOSE:
            release(&tcpsock->spinlk);
            tcp_done(tcpsock);
            return 0;
            break;
        case TCP_LISTEN:
            tcp_clear_listen_queue(tcpsock);
            release(&tcpsock->spinlk);
            tcp_done(tcpsock);
            return 0;
        case TCP_SYN_RECEIVED:
        case TCP_SYN_SENT:
            tcp_done(tcpsock);
            break;
        case TCP_ESTABLISHED:
            tcp_set_state(tcpsock, TCP_FIN_WAIT_1);
            tcp_send_fin(tcpsock);
            tcpsock->tcb.send_next += 1;
            bread;
        case TCP_CLOSE_WAIT:
            tcp_set_state(tcpsock, TCP_LAST_ACK);
            tcp_send_fin(tcpsock);
            tcpsock->tcb.send_next += 1;
            break;
    }
    
    release(&tcpsock->spinlk);
    return 0;
}