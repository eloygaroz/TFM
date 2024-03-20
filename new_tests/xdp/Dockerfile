FROM ubuntu:20.04

WORKDIR /app
RUN apt-get update \
 && apt-get install -y wget clang llvm
# Install necessary development tools and kernel headers
#RUN apt install -y clang-12 --install-suggests 
# Copy your eBPF program source code
COPY xdp.c .
RUN wget https://aka.pw/bpf-ecli -O ecli && chmod +x ecli
RUN wget https://github.com/eunomia-bpf/eunomia-bpf/releases/latest/download/ecc -O ecc && chmod +x ecc
#CMD ["./ecc","minimal.bpf.c"]
#CMD ["./ecli", "run", "package.json"]

