FROM ubuntu:20.04

WORKDIR /app
RUN apt-get update \
 && apt-get install -y iptables 
# Install necessary development tools and kernel headers
#RUN apt install -y clang-12 --install-suggests 
# Copy your eBPF program source code
#CMD ["./ecc","minimal.bpf.c"]
#CMD ["./ecli", "run", "package.json"]

