#pragma once 

#include "sysheaders.h"
#include "mbuf.h"
#include "list.h"
#include "ip.h"

#define TCP_HDR_LEN sizeof(struct tcp_hdr)
#define TCP_DOFFSET sizeof(struct tcp_hdr) / 4

struct tcp_hdr {
    uint16_t src_port;
    uint16_t dst_port;
    uint32_t seq;
    uint32_t ack_seq;
    uint32_t reserved : 4, 
            data_offset : 4;      
    uint32_t fin : 1,
            syn : 1,
            rst : 1,
            psh : 1,
            ack : 1,
            urg : 1,
            ece : 1,
            cwr : 1;
    uint16_t window;
    uint16_t checksum;
    uint16_t urgptr;
};

enum tcp_states {
    TCP_LISTEN, TCP_SYN_SENT, TCP_SYN_RECEIVED, TCP_ESTABLISHED, 
    TCP_FIN_WAIT_1, TCP_FIN_WAIT_2,
    TCP_CLOSE, TCP_CLOSE_WAIT, TCP_CLOSING, 
    TCP_LAST_ACK, TCP_TIME_WAIT
};


struct tcb {
    uint32_t send_unack;
    uint32_t send_next;
    uint32_t send_window;
    uint32_t send_urgent;
    uint32_t send_win1;
    uint32_t send_win1;
    uint32_t iss;        // initial send sequence number

    uint32_t recv_next;
    uint32_t recv_window;
    uint32_t recv_urgent;
    uint32_t irs;        // initial receive sequence number
};

struct tcp_sock {
    struct list_head tcpsock_list;
    uint32_t src_addr;
    uint32_t dst_addr;
    uint16_t src_port;
    uint16_t dst_port;

    int backlog;
    int accepted_backlog;

    uint32_t state;
    struct tcb tcb;

};

void tcp_rx(struct mbuf* m, uint16_t len, struct ip_hdr* iphdr);
void tcp_tx(struct tcp_sock *ts, struct tcp_hdr *tcphdr, struct mbuf *m, uint16_t seq);

// functions for send
// special packets
void tcp_send_reset(struct tcp_sock *ts);
void tcp_send_syn(struct tcp_sock *ts);
void tcp_send_fin(struct tcp_sock *ts);
void tcp_send_ack(struct tcp_sock *ts);
void tcp_send_synack(struct tcp_sock *ts);
// main data tx function
int tcp_send(struct tcp_sock *ts, char *buffer, int len);


// functions for rx