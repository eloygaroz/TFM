
#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include "xdp.h"


struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} rb SEC(".maps");

SEC("xdp")
int xdp_pass(struct xdp_md* ctx) {
    // Capture packet size and meta size
    void* data = (void*)(long)ctx->data;
    void* data_end = (void*)(long)ctx->data_end;
    void* data_meta = (void*)(long)ctx->data_meta;
    int pkt_sz = data_end - data;
    int meta_sz = data - data_meta;
    struct ethhdr *eth = data;
    if (data + sizeof(struct ethhdr) > data_end){
        return XDP_DROP;
    }

    struct event *e;
    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (e) {
        e->pkt_size = pkt_sz;
        e->in_ifce = ctx->ingress_ifindex;
        //memcpy(e->h_dst, eth->h_dest, 6);
        //memcpy(e->h_src, eth->h_source, 6);
        e->h_proto = eth->h_proto;
        // Submit the event to the ring buffer
        bpf_ringbuf_submit(e, 0);
    }

    return XDP_PASS;
}

char __license[] SEC("license") = "GPL";
