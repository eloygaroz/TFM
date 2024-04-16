#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include "tc.h"
#define TC_ACT_OK 0
#define ETH_P_IP 0x0800 /* Internet Protocol packet */
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} rb SEC(".maps");
/// @tchook {"ifindex":2, "attach_point":"BPF_TC_INGRESS"}
/// @tcopts {"handle":1, "priority":1}
SEC("tc")
int tc_ingress(struct __sk_buff *skb)
{
    void *data_end = (void *)(long)skb->data_end;
    void *data = (void *)(long)skb->data;

    struct ethhdr *eth = data;
    if (data + sizeof(struct ethhdr) > data_end) {
        return TC_ACT_OK;
    }
    if (eth->h_proto != __builtin_bswap16(ETH_P_IP)) {
        return XDP_PASS; // Not IP, ignore
    }
    // Move data pointer past the Ethernet header
    data += sizeof(struct ethhdr);
    if (data > data_end) {
        return TC_ACT_OK;
    }

    struct iphdr *ip = data;
    if (data + sizeof(struct iphdr) > data_end) {
        return TC_ACT_OK;
    }

    // Move data pointer past the IP header
    data += sizeof(struct iphdr);
    if (data > data_end) {
        return TC_ACT_OK;
    }

    struct event *e;
    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (e) {
        unsigned int src_ip = __builtin_bswap32(ip->saddr);
        unsigned int dst_ip = __builtin_bswap32(ip->daddr);

        e->src_ip[0] = (src_ip >> 24) & 0xFF;
        e->src_ip[1] = (src_ip >> 16) & 0xFF;
        e->src_ip[2] = (src_ip >> 8) & 0xFF;
        e->src_ip[3] = src_ip & 0xFF;
	 
	e->dst_ip[0] = (dst_ip >> 24) & 0xFF;
        e->dst_ip[1] = (dst_ip >> 16) & 0xFF;
        e->dst_ip[2] = (dst_ip >> 8) & 0xFF;
        e->dst_ip[3] = dst_ip & 0xFF;
        
	e->pkt_size = skb->len;
        e->in_ifce = skb->ifindex;
        e->type = ip->protocol;
        //e->src = __builtin_bswap32(ip->saddr);
        //e->dest = __builtin_bswap32(ip->daddr);
        if (ip->protocol == IPPROTO_TCP) {
            struct tcphdr *tcp = data;
            if (data + sizeof(struct tcphdr) > data_end) {
                bpf_ringbuf_discard(e, 0);
                return TC_ACT_OK;
            }
            e->sport = __builtin_bswap16(tcp->source);
            e->dport = __builtin_bswap16(tcp->dest);
        } else if (ip->protocol == IPPROTO_UDP) {
            struct udphdr *udp = data;
            if (data + sizeof(struct udphdr) > data_end) {
                bpf_ringbuf_discard(e, 0);
                return TC_ACT_OK;
            }
            e->sport = __builtin_bswap16(udp->source);
            e->dport = __builtin_bswap16(udp->dest);
        }

        // Output the stripped packet
        bpf_ringbuf_submit(e, 0);
    }

    return TC_ACT_OK;
}

char __license[] SEC("license") = "GPL";
