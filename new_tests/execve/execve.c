// SPDX-License-Identifier: BSD-3-Clause
#include "vmlinux.h"
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_tracing.h>
#include <bpf/bpf_core_read.h>
#include "signal.h"

char LICENSE[] SEC("license") = "Dual BSD/GPL";

// Ringbuffer Map to pass messages from kernel to user
struct {
    __uint(type, BPF_MAP_TYPE_RINGBUF);
    __uint(max_entries, 256 * 1024);
} rb SEC(".maps");


SEC("tp/syscalls/sys_enter_execve")
int bpf_prog1(struct trace_event_raw_sys_enter *ctx)
{
    struct event *e;
    size_t pid_tgid = bpf_get_current_pid_tgid();
    int pid = pid_tgid >> 32;

    // Log event
    e = bpf_ringbuf_reserve(&rb, sizeof(*e), 0);
    if (e) {
        e->success = true; // If we're here, execve was called
        e->pid = pid;
        bpf_get_current_comm(&e->comm, sizeof(e->comm));
        bpf_ringbuf_submit(e, 0);
    }

    return 0;
}

char _license[] SEC("license") = "GPL";

