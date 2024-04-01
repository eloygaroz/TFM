struct event {
    int pkt_size;         // Packet size
    int in_ifce;  // Ingress interface index
    //unsigned char h_dst[6];
    //unsigned char h_src[6];
    //__u32 src;
    //__u32 dest;
    __u16 type;
    __be16 h_proto;

}_attribute_((packed));
