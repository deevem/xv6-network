#include "sysheaders.h"
#include "../memlayout.h"
#include "../spinlock.h"
#include "../proc.h"
#include "../fs.h"
#include "../sleeplock.h"
#include "../file.h"
#include "socket.h"
#include "ip.h"
#include "udp.h"
#include "icmp.h"

static struct spinlock lock;
static struct socket* socks;

void sockinit(void)
{
    initlock(&lock, "socktbl");
}

// give a socket to a udp and a process
int udp_sockalloc(struct file **f, uint32 r_ip, uint16 lport, uint16 rport)
{
    struct socket * si, *pos;

    si = 0;
    *f = 0;
    if ((*f = filealloc()) == 0)
        goto bad;
    if ((si = (struct socket*)kalloc()) == 0)
        goto bad;
    
    si->protocol_type = IPPROTO_UDP;
    si->r_ip = r_ip;
    si->lport = lport;
    si->rport = rport;
    initlock(&si->lock, "sock");

    (*f)->type = FD_SOCK;
    (*f)->readable = 1;
    (*f)->writable = 1;
    (*f)->sock = si;


    acquire(&lock);
    pos = socks;
    while (pos){
        if (pos->r_ip == r_ip && pos->lport == lport && pos->rport == rport)
        {
            release(&lock);
            goto bad;
        }
        pos = pos->next;
    }
    si->next = socks;
    socks = si;
    release(&lock);
    return 0;
bad:
    if (si)
        kfree((char*)si);
    if (*f)
        fileclose(*f);
    return -1;
}

int icmp_sockalloc(struct file ** f, uint32 r_ip, uint8 icmp_type, uint8 icmp_code){
    struct socket *si, *pos;

    si = 0;
    *f = 0;
    if ((*f = filealloc()) == 0)
        goto bad;
    if ((si = (struct socket*)kalloc()) == 0)
        goto bad;
    
    si->protocol_type = IPPROTO_ICMP;
    si->r_ip = r_ip;
    si->icmp_type = icmp_type;
    si->icmp_code = icmp_code;
    initlock(&si->lock, "sock");
    (*f)->type = FD_SOCK;
    (*f)->readable = 1;
    (*f)->writable = 1;
    (*f)->sock = si;

    acquire(&lock);
    pos = socks;
    while (pos)
    {
        if (pos->protocol_type == IPPROTO_ICMP && pos->r_ip == r_ip)
        {
            release(&lock);
            goto bad;
        }
        pos = pos->next;
    }
    si->next = socks;
    socks = si;
    release(&lock);
bad:
    if (si)
        kfree((char*)si);
    if (*f)
        fileclose(*f);
    return -1;
}

void sockclose(struct socket * si)
{
    struct socket ** pos;
    struct mbuf *m;

    acquire(&lock);
    pos = &socks;
    while(*pos){
        if (*pos == si)
        {
            *pos = si->next;
            break;
        }
        pos = &(*pos)->next;
    }
    release(&lock);

    while (!mbuf_queue_empty(&si->rxq))
    {
        m = mbuf_queue_pophead(&si->rxq);
        mbuffree(m);
    }

    kfree((char*)si);
}

int sockread(struct socket *si, uint64 addr, int n)
{
    struct proc *pr = myproc();
    struct mbuf *m;
    int len;


    acquire(&si->lock);
    while (mbuf_queue_empty(&si->rxq) && !pr->killed)
    {
        sleep(&si->rxq, &si->lock);
    }
    if (pr->killed)
    {
        release(&si->lock);
        return -1;
    }
    

    printf("socket read %d %d \n", &si->rxq.head, &si->rxq.tail);   
    
    m = mbuf_queue_pophead(&si->rxq);


    release(&si->lock);

    len = m->len;
    if (len > n)
        len = n;
    if (copyout(pr->pagetable, addr, m->head, len) == -1){
        mbuffree(m);
        return -1;
    }
    mbuffree(m);
    return len;
}

int sockwrite(struct socket *si, uint64 addr, int n)
{
    struct proc * pr = myproc();
    struct mbuf *m;

    m = mbufalloc(MBUF_DEFAULT_HEADROOM);
    if (!m)
        return -1;
    if (copyin(pr->pagetable, mbufput(m, n), addr, n) == -1)
    {
        mbuffree(m);
        return -1;
    }
    if (si->protocol_type == IPPROTO_UDP) {
        udp_tx(m, si->r_ip, si->lport, si->rport);
        printf("udp sent socket success\n");
    }
    else if (si->protocol_type == IPPROTO_ICMP)
        icmp_tx(m, si->r_ip, si->icmp_type, si->icmp_code);
    return n;
}

void sockrecvudp(struct mbuf *m, uint32 r_ip, uint16 lport, uint16 rport)
{
    struct socket *si;
    acquire(&lock);
    si = socks;
    while (si)
    {
        if (si->protocol_type == IPPROTO_UDP && si->r_ip == r_ip && si->lport == lport && si->rport == rport)
            goto found;
        si = si->next;
    }
    release(&lock);
    mbuffree(m);
    return;
found:
    acquire(&si->lock);
    mbuf_queue_pushtail(&si->rxq, m);
    wakeup(&si->rxq);
    release(&si->lock);
    release(&lock);
}

void sockrecvicmp(struct mbuf *m, uint32 r_ip, uint8 ttl)
{
    struct socket *si;
    acquire(&lock);
    si = socks;
    while (si)
    {
        if (si->protocol_type == IPPROTO_ICMP && si->r_ip == r_ip)
            goto found;
        si = si->next;
    }
    release(&lock);
    mbuffree(m);
    return;
found:
    si->icmp_recvttl = 59;
    acquire(&si->lock);
    mbuf_queue_pushtail(&si->rxq, m);
    wakeup(&si->rxq);
    release(&si->lock);
    release(&lock);
}