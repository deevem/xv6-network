#include "tcp.h"
#include "utils.h"
#include "../proc.h"

struct tcp_sock * tcp_sock_lookup_establish(uint32_t src, uint32_t dst, uint16_t src_port, uint16_t dst_port) {
    struct tcp_sock *tcpsock = NULL, *s;
    acquire(&tcpsocks_list_lk);
    list_for_each_entry(s, &tcpsocks_list_head, tcpsock_list) {
        if (src == s->dst_addr && src_port == s->dst_port && dst_port == s->src_port) {
            tcpsock = s;
            break;
        }
    }
    release(&tcpsocks_list_lk);
    return tcpsock;
}

struct tcp_sock * tcp_sock_lookup_listen(uint32_t dst, uint16_t dst_port) {
    struct tcp_sock *tcpsock = NULL, *s;
    acquire(&tcpsocks_list_lk);
    list_for_each_entry(s, &tcpsocks_list_head, tcpsock_list) {
        if (dst_port == s->src_port && s->state == TCP_LISTEN) {
            tcpsock = s;
            break;
        }
    }
    release(&tcpsocks_list_lk);
    return tcpsock;
}

struct tcp_sock * tcp_sock_lookup(uint32_t src, uint32_t dst, uint16_t src_port, uint16_t dst_port) {
    struct tcp_sock *tcpsock = NULL;
    tcpsock = tcp_sock_lookup_establish(src, dst, src_port, dst_port);
    if (tcpsock == NULL) 
        tcpsock = tcp_sock_lookup_listen(dst, dst_port);
    return tcpsock;
}

uint32 
sum_every_16bits(void *addr, int count)
{
    register uint32 sum = 0;
    uint16 * ptr = addr;
    
    while( count > 1 )  {
        /*  This is the inner loop */
        sum += * ptr++;
        count -= 2;
    }

    /*  Add left-over byte, if any */
    if( count > 0 )
        sum += * (uint8 *) ptr;

    return sum;
}

uint16 
checksum1(void *addr, int count, int start_sum)
{
    /* Compute Internet Checksum for "count" bytes
     *         beginning at location "addr".
     * Taken from https://tools.ietf.org/html/rfc1071
     */
    uint32 sum = start_sum;

    sum += sum_every_16bits(addr, count);
    
    /*  Fold 32-bit sum to 16 bits */
    while (sum>>16)
        sum = (sum & 0xffff) + (sum >> 16);

    return ~sum;
}

int
tcp_v4_checksum(struct mbuf *m, uint32 saddr, uint32 daddr)
{
  uint32 sum = 0;
  
  sum += saddr;
  sum += daddr;
  sum += htons(IPPROTO_TCP);
  sum += htons(m->len);

  return checksum1(m->head, m->len, sum);
}


void tcp_sock_tx(struct tcp_sock *tcpsock, struct tcp_hdr *tcphdr, struct mbuf *m, uint32_t seq) {
    tcphdr->data_offset = TCP_DOFFSET;
    tcphdr->src_port = htons(tcpsock->src_port);
    tcphdr->dst_port = htons(tcpsock->dst_port);
    tcphdr->seq = htonl(seq);
    tcphdr->ack_seq = htonl(tcpsock->tcb.recv_next);
    tcphdr->reserved = htons(0);
    tcphdr->window = htons(tcpsock->tcb.recv_window);
    tcphdr->checksum = tcp_v4_checksum(m, htonl(tcpsock->src_addr), htonl(tcpsock->dst_addr));
    tcphdr->urg = htons(0);

    ip_tx(m, IPPROTO_TCP, tcpsock->dst_addr);
}

void tcp_rx(struct mbuf* m, uint16_t len, struct ip_hdr* iphdr) {
    struct tcp_hdr *tcphdr;
    tcphdr = (struct tcp_hdr*)mbufpull(m, sizeof(struct tcp_hdr));

    if (tcphdr->data_offset > TCP_MIN_DATA_OFF) 
        mbufpull(m, 4 * (tcphdr->data_offset - TCP_MIN_DATA_OFF));

    tcphdr->src_port = ntohs(tcphdr->src_port);
    tcphdr->dst_port = ntohs(tcphdr->dst_port);
    tcphdr->seq = ntohl(tcphdr->seq);
    tcphdr->ack_seq = ntohl(tcphdr->ack_seq);
    tcphdr->window = ntohs(tcphdr->window);
    tcphdr->checksum = ntohs(tcphdr->checksum);
    tcphdr->urg = ntohs(tcphdr->urg);
    
    uint32_t src = ntohl(iphdr->src_addr);
    uint32_t dst = ntohl(iphdr->dst_addr);

    m->start_seq = tcphdr->seq;
    m->end_seq = tcphdr->seq + m->len;

    struct tcp_sock *tcpsock = tcp_sock_lookup(src, dst, tcphdr->src_port, tcphdr->dst_port);

    if (tcpsock == NULL) {
        mbuffree(m);
        return;
    }
    printf("tcp sock found! \n");


    acquire(&tcpsock->spinlk);
    int r = tcp_input_state(tcpsock, tcphdr, iphdr, m);
    
    if (r == 0) 
        release(&tcpsock->spinlk);

}


void tcp_send_syn(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;
    
    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 1;

    tcp_sock_tx(tcpsock, tcphdr, m, tcpsock->tcb.iss);
}

void tcp_send_synack(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;

    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 1;
    tcphdr->ack = 1;

    tcp_sock_tx(tcpsock, tcphdr, m, tcpsock->tcb.iss);
}

void tcp_send_ack(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;

    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->syn = 0;
    tcphdr->ack = 1;
    tcp_sock_tx(tcpsock, tcphdr, m, tcpsock->tcb.send_next);
}

void tcp_send_reset(struct tcp_sock *tcpsock){
    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->rst = 1;
    tcpsock->tcb.send_unack = tcpsock->tcb.send_next; 

    tcp_sock_tx(tcpsock, tcphdr, m, tcpsock->tcb.send_next);
}
void tcp_send_fin(struct tcp_sock *tcpsock) {
    if (tcpsock->state == TCP_CLOSE)
        return;

    struct mbuf *m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    struct tcp_hdr *tcphdr = (struct tcp_hdr *)mbufpush(m, sizeof(struct tcp_hdr));
    tcphdr->ack = 1;
    tcphdr->fin = 1;
    
    tcp_sock_tx(tcpsock, tcphdr, m, tcpsock->tcb.send_next);
}

int tcp_send(struct tcp_sock *tcpsock, uint64_t buffer, int len) {
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
        rest_len -= packet_len;
        if (rest_len == 0)
            tcphdr->psh = 1;
        
        tcp_sock_tx(tcpsock, tcphdr, m, tcpsock->tcb.send_next);
        tcpsock->tcb.send_next += packet_len;
    }

    return len - rest_len;
}


void tcp_set_state(struct tcp_sock *tcpsock, enum tcp_states state) {
    tcpsock->state = state;
}

void tcp_free(struct tcp_sock *tcpsock) {
    acquire(&tcpsocks_list_lk);
    list_del(&tcpsock->tcpsock_list);
    release(&tcpsocks_list_lk);
}

void tcp_sock_free(struct tcp_sock *tcpsock) {
    kfree(tcpsock);
}

void tcp_done(struct tcp_sock *tcpsock) {
    tcp_set_state(tcpsock, TCP_CLOSE);
    tcp_free(tcpsock);
    tcp_sock_free(tcpsock);
}

uint32_t alloc_new_iss(void) {
	static unsigned int iss = 12345678;
	if (++iss >= 0xffffffff)
		iss = 12345678;
    printf("%d\n", iss);
	return iss;
}
