#include "list.h"
#include "tcp.h"


static void tcp_data_queue(struct tcp_sock* ts, struct tcp_hdr* th, struct mbuf* m){
    if(!ts->tcb.recv_window){
        return -1;
    }

    if(m.se)
}