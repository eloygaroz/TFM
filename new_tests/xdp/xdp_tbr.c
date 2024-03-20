#include "vmlinux.h"
#include <bpf/bpf_helpers.h>

#ifndef ETH_P_IP
#define ETH_P_IP    0x0800  // IPv4 protocol
#endif

// Define bpf_ntohs if it is not available
static __always_inline __u16 bpf_ntohs(__u16 x) {
    return __builtin_bswap16(x);
}

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} rb SEC(".maps");

struct event {
    int pkt_sz;
    unsigned short eth_proto;
    bool is_ipv4;
};

SEC("xdp")
int xdp_inspect(struct xdp_md* ctx) {
    void* data = (void*)(long)ctx->data;
    void* data_end = (void*)(long)ctx->data_end;
    struct ethhdr *eth = data;
    struct event *e;

    if ((void*)(eth + 1) > data_end)
        return XDP_PASS;

    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (!e) {
        return XDP_PASS;
    }

    e->pkt_sz = data_end - data;
    e->eth_proto = bpf_ntohs(eth->h_proto);
    e->is_ipv4 = (e->eth_proto == ETH_P_IP);

    bpf_ringbuf_submit(e, 0);

    return XDP_PASS;
}

char __license[] SEC("license") = "GPL";

