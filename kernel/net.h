#define MBUF_SIZE 2048
#define MHBUF_SIZE 100  // the max length of buf with a header of packet
#define MBUF_DEFAULT_HEADROOM 128

// -------------------- defs of mbuf ---------------------

struct m_hdr {
    struct mbuf* mh_next;
    struct mbuf* mh_nextpkt;
    unsigned int mh_len;
    char*        mh_data;
    short        mh_type;
    short        mh_flags;
};

struct pkthdr {
    int           len;
    struct ifnet* rcvif;
};

struct mbuf {
    struct m_hdr m_hdr;
    union {
        struct {
            struct pkthdr MH_pkthder;
            char          MH_databuf[MHBUF_SIZE];
        } MH;
        char M_databuf[MBUF_SIZE];
    };
};


// ----------- headers of net protocal ------------------


#define ETHADDR_LEN 6

// the values of eth_hdr
#define ETHTYPE_IP 0x0800
#define ETHTYPE_ARP 0x0806

// header of ethernet protocol
struct eth_hdr {
    uint8  dsthost[ETHADDR_LEN];
    uint8  srchort[ETHADDR_LEN];
    uint16 type;
};

// values of ip_hdr.ip_p

#define IPPROTO_ICMP 1  // Control message protocol
#define IPPROTO_TCP 6   // Transmission control protocol
#define IPPROTO_UDP 17  // User datagram protocol

// header of ip protocol
struct ip_hdr {
    uint8  ip_vhl;
    uint8  ip_tos;  // type of service
    uint16 ip_len;
    uint16 ip_id;
    uint16 ip_off;
    uint8  ip_ttl;
    uint8  ip_p;
    uint8  ip_sum;
    uint32 ip_src, ip_dst;
};

inline uint32 MAKE_IP_ADDR(uint8 a, uint8 b, uint8 c, uint8 d) {
    return (a << 24) | (b << 16) | (c << 8) | (d);
}

struct udp {
    uint16 sport;  // source port
    uint16 dport;
    uint16 ulen;
    uint16 sum  // check sum of udp
};

struct arp {
    uint16 hrd;
    uint16 pro;
    uint8  hln;
    uint8  pln;
    uint16 op;

    // sender hardware address
    char sha[ETHADDR_LEN];
    // sender IP
    uint32 sip;
    // target hardware address
    char tha[ETHADDR_LEN];
    // target IP
    uint32 tip;
} __attribute__((packed));
