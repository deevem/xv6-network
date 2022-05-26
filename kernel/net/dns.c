#include "dns.h"

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

int dns_request(char* buf, const char* host) {
    // len of the dns request
    int             len = sizeof(struct dns_hdr);
    struct dns_hdr* dnshdr = ( struct dns_hdr* )buf;

    dnshdr->id = htons(6828);
    dnshdr->rd = 1;
    dnshdr->qdcount = htons(1);

    // gee qname of the question
    char* qname = buf + sizeof(struct dns_hdr);

    qname_encoder(qname, host);
    len += strlen(qname) + 1;

    struct dns_question* h = ( struct dns_question* )(qname + strlen(qname) + 1);
    h->qtype = htons(0x1);
    h->qclass = htons(0x1);

    len += sizeof(struct dns_question);
    return len;
}

int dns_response(char* buf, int recv_len) {
    struct dns_hdr* dnshdr = ( struct dns_hdr* )buf;
    char*           qname = NULL;
    int             record = 0;

    // TODO: check qr, id, rcode
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
        printf("dns len %d \n", len);

        qn = (char *)(buf+qn[1]);
        len += 2;

        struct dns_data* d = ( struct dns_data* )(buf + len);
        len += sizeof(struct dns_data);

        printf("d type %d\n", ntohs(d->type));

        if (ntohs(d->type) == ARECORD && ntohs(d->len) == 4) {
            record = 1;
            printf("DNS arecord for %s is ", qname);
            uint8_t* ip = (uint8_t*)(buf + len);
            printf("%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
            len += 4;
        }
    }
    printf("dns finished \n");
    // TODO : check recv_len, record

    // ..... 
    return record;
}
