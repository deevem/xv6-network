#include "user/dns.h"
#include "user/user.h"

#define MAKE_IP_ADDR(a, b, c, d) (((uint32_t)a << 24) | ((uint32_t)b << 16) | ((uint32_t)c << 8) | (uint32_t)d)

/*
    from :abcd.efgh.ijk.lmno
    to   :4abcd4eghh3ijk4lmno
*/
static void qname_encoder(char* qname, const char* host) {
    int pos = 0;
    for (int i = 0; i < strlen(host); i++) {
        if (host[i] != '.')
            qname[i + 1] = host[i];
        else {
            qname[pos] = i - pos;
            pos = i + 1;
        }
    }
}

static void decode_qname(char* qn) {
    while (*qn != '\0') {
        int l = *qn;
        if (l == 0)
            break;
        for (int i = 0; i < l; i++) {
            *qn = *(qn + 1);
            qn++;
        }
        *qn++ = '.';
    }
}

int dns_request(uint8* buf, const char* host) {
    // len of the dns request
    int             len = sizeof(struct dns_hdr);
    struct dns_hdr* dnshdr = ( struct dns_hdr* )buf;

    dnshdr->id = htons(6828);
    dnshdr->rd = 1;
    dnshdr->qdcount = htons(1);

    // gee qname of the question
    char* qname =(char*) (buf + sizeof(struct dns_hdr));

    qname_encoder(qname, host);
    len += strlen(qname) + 1;

    struct dns_question* h = ( struct dns_question* )(qname + strlen(qname) + 1);
    h->qtype = htons(0x1);
    h->qclass = htons(0x1);

    len += sizeof(struct dns_question);
    return len;
}

int dns_response(uint8* buf, int recv_len) {
    struct dns_hdr* dnshdr = ( struct dns_hdr* )buf;
    char*           qname = NULL;
    int             record = 0;
    uint32 ip_receive = 0;

    if(!dnshdr->qr) {
        printf("Not a DNS response for %d\n", ntohs(dnshdr->id));
        exit(1);
    }

    if(dnshdr->id != htons(6828))
        printf("DNS wrong id: %d\n", ntohs(dnshdr->id));

    if(dnshdr->rcode != 0) {
        printf("DNS rcode error: %x\n", dnshdr->rcode);
        exit(1);
    }

    int len = sizeof(struct dns_hdr);

    // question entries
    for (int i = 0; i < ntohs(dnshdr->qdcount); i++) {
        char* qn = ( char* )(buf + len);
        qname = qn;
        decode_qname(qn);
        len += strlen(qn) + 1;
        len += sizeof(struct dns_question);
        printf("dns question %s \n", qname);
    }


    // answer section
    for (int i = 0; i < ntohs(dnshdr->ancount); i++) {
        char* qn = ( char* )(buf + len);

        if((int) qn[0] > 63) {
            qn = (char *)(buf+qn[1]);
            len += 2;
        } else {
            decode_qname(qn);
            len += strlen(qn)+1;
        }

        struct dns_data* d = ( struct dns_data* )(buf + len);
        len += sizeof(struct dns_data);

        if (ntohs(d->type) == ARECORD && ntohs(d->len) == 4) {
            record = 1;
            uint8_t* ip = (uint8_t*)(buf + len);
            printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
            // ip_receive = ((uint32_t)(ip[0]) << 24) + ((uint32_t)(ip[1]) << 16) + ((uint32_t)(ip[2]) << 8) + ip[3];
            ip_receive = MAKE_IP_ADDR(ip[0], ip[1], ip[2], ip[3]);
            len += 4;
        }
    }

    if(len != recv_len) {
        printf("Processed %d data bytes but received %d\n", len, recv_len);
        exit(1);
    }
    if(!record) {
        printf("Didn't receive an arecord\n");
        exit(1);
    }
    // ..... 
    return ip_receive;
}

uint32 dns(char* ss)
{
    char s[100];
    memset(s, 0, sizeof(s));
    memmove(s, ss, strlen(ss));
    
    s[strlen(s) + 1] = '\0';
    s[strlen(s)] = '.';
    
    #define N 200
    uint8 obuf[N];
    uint8 ibuf[N];
    uint32 dst;
    int fd;
    int len;

    memset(obuf, 0, N);
    memset(ibuf, 0, N);

    dst = (114 << 24) | (114 << 16) | (114 << 8) | (114 << 0);

    if((fd = connect(dst, 12345, 53)) < 0){
        printf("dns: connect() failed\n");
        exit(1);
    }

    len = dns_request(obuf, s);

    if (write(fd, obuf, len) < 0){
        printf( "dns: send() failed\n");
        exit(1);
    }

    
    int cc = read(fd, ibuf, sizeof(ibuf));

    if (cc < 0)
    {
        printf( "dns: recv() failed\n");
        exit(1);
    }

    uint32 ans = dns_response(ibuf, cc);

    close(fd);
    return ans;
}