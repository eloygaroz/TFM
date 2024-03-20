#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include "xdp.h"
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>

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

    //size_t pid_tgid = bpf_get_current_pid_tgid();
    // Reserve space in the ring buffer for an event
    struct event *e;
    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (e) {
        e->pkt_size = pkt_sz;
        e->meta_size = meta_sz;
        e->ingress_ifindex = ctx->ingress_ifindex;
        // Getting process ID and command name
        //e->pid = pid_tgid >> 32;
        //bpf_get_current_comm(&e->comm, sizeof(e->comm));
        // Getting CPU core identifier
        e->cpu_core = bpf_get_smp_processor_id();

        // Submit the event to the ring buffer
        bpf_ringbuf_submit(e, 0);
    }

    return XDP_PASS;
}

char __license[] SEC("license") = "GPL";

