#include <vmlinux.h>
#include <bpf/bpf_endian.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include "tc.h"


#define TC_ACT_OK 0
#define ETH_P_IP 0x0800 /* Internet Protocol packet */

struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} rb SEC(".maps");


/// @tchook {"ifindex":1, "attach_point":"BPF_TC_INGRESS"}
/// @tcopts {"handle":1, "priority":1}
SEC("tc")
int tc_ingress(struct __sk_buff *ctx)
{
    void *data_end = (void *)(long)ctx->data_end;
    void *data = (void *)(long)ctx->data;
    void *pkt_type = (void *)(long)ctx->pkt_type;
    void *protocol = (void *)(long)ctx->protocol;
    void *ingress_ifindex = (void *)(long)ctx->ingress_ifindex;
    void *ifindex = (void *)(long)ctx->ifindex;

    void *tc_index = (void *)(long)ctx->tc_index;
    struct ethhdr *l2;
    struct iphdr *l3;
    if (ctx->protocol != bpf_htons(ETH_P_IP))
        return TC_ACT_OK;

    l2 = data;
    if ((void *)(l2 + 1) > data_end)
        return TC_ACT_OK;

    l3 = (struct iphdr *)(l2 + 1);
    if ((void *)(l3 + 1) > data_end)
        return TC_ACT_OK;
    int pkt_sz = data_end - data;
    struct event *e;
    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (e) {
        e->len = pkt_sz;
        e->pkt_type = pkt_type;
        e->protocol = protocol;
        e->ingress_ifindex = ingress_ifindex;
        e->ifindex = ifindex;
	e->tc_index = tc_index;
        bpf_ringbuf_submit(e, 0);
        return TC_ACT_OK;
        }
    }
char __license[] SEC("license") = "GPL";
