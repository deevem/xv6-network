#include "tcp.h"
#include "mbuf.h"
#include "../proc.h"
#include "list.h"

static void tcp_consume_ofo_queue(struct tcp_sock* sock){//move first mbuf in ofo_queue into rcv_queue when in order
    struct mbuf* m = NULL;
    while ((m = tcp_mbuf_queue_peek(&sock->ofo_queue)) != NULL && sock->tcb.recv_next == m->start_seq){
        sock->tcb.recv_next += m->len;
        tcp_mbuf_dequeue(&sock->ofo_queue);
        tcp_mbuf_enqueue(&sock->rcv_queue,m);
    }
}

static void tcp_check_ofo_queue(struct tcp_sock* sock, struct mbuf* m){
    struct mbuf *n;
    
    list_for_each_entry(n,&sock->ofo_queue.head, list){
        if(m->start_seq < n->start_seq){
            if(m->end_seq > n->start_seq){
                m->len = m->len - (m->end_seq - n->end_seq);
            }
            m->refcnt++;
            tcp_mbuf_enqueue(&sock->ofo_queue,m);
            return;
        }
        else{
            return;
        }

    }

}


int tcp_data_queue(struct tcp_sock* sock, struct mbuf* m){

    if(!sock->tcb.recv_window){
        return -1;
    }

    if(m->start_seq == sock->tcb.recv_next){
        sock->tcb.recv_next += m->len;
        m->refcnt++;
        tcp_mbuf_enqueue(&sock->rcv_queue,m);

        tcp_consume_ofo_queue(sock);

        wakeup(&sock->wait_rcv);
    } else {
        tcp_check_ofo_queue(sock,m);
    }

    tcp_send_ack(sock);
    return 0;
}

int tcp_data_dequeue(struct tcp_sock* sock, uint64 ubuf, int len, int* psh){
    struct tcp_hdr* th;
    int rlen = 0;

    while(!tcp_mbuf_queue_empty(&sock->rcv_queue) && rlen < len){
        struct mbuf* m = tcp_mbuf_queue_peek(&sock->rcv_queue);
        if(m == NULL) break;

        th = (struct tcp_hdr*)(m->head - sizeof(*th));

        int dlen = (rlen + m->len) > len ? (len-rlen) : m->len;
        copyout(myproc()->pagetable,ubuf,m->head,dlen);

        m->len -= dlen;
        m->head += dlen;
        rlen += dlen;
        ubuf += dlen;
        
        if(m->len == 0){
            if(th->psh) *psh = 1;
            tcp_mbuf_dequeue(&sock->rcv_queue);
            mbuffree(m);
        }
    }

    return rlen;
}