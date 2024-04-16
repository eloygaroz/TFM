struct event {
    int pkt_size;         // Packet size
    int in_ifce;          // Ingress interface index
    __u8 type;            // Protocol type (TCP/UDP)
    __be16 sport;      // Source port (for TCP/UDP)
    __be16 dport;     // Destination port (for TCP/UDP)
    //__be32 src;
    //__be32 dest;
    unsigned char src_ip[4];
    unsigned char dst_ip[4];
} __attribute__((packed));
