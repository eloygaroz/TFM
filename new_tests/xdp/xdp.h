struct event {
    int pkt_size;         // Packet size
    int meta_size;        // Size of metadata
    int ingress_ifindex;  // Ingress interface index
    int pid;              // Process ID
    char comm[16]; // Command name of the process
    int cpu_core;         // CPU core identifier
    //int egress_ifindex;
};
