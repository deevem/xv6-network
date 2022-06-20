#include "list.h"
#include "tcp.h"

static void tcp_consume_ofo_queue(struct tcp_sock* sock){
    struct mbuf* m = NULL;
    while ((m = tcp_mbuf_queue_peek(sock.)))
}
static void tcp_data_queue(struct tcp_sock* sock, struct tcp_hdr* th, struct mbuf* m){
    if(!ts->tcb.recv_window){
        return -1;
    }

    if(m.se)
}