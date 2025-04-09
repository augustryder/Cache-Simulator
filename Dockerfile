# Dockerfile for a development environment with gcc, gdb, valgrind, and python3

# Use the latest Ubuntu image
FROM ubuntu:latest

# Set the working directory
WORKDIR /work

# Install dependencies
RUN apt-get update && \
    apt-get install -y build-essential \
    valgrind \
    gdb \
    python3 && \
    apt-get clean

CMD ["/bin/bash"]

