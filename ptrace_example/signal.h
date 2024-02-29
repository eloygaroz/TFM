// Simple message structure to get events from eBPF Programs
// in the kernel to user space
#define TASK_COMM_LEN 16
struct event {
    int pid;
    char comm[TASK_COMM_LEN];
    bool success;
};
