# CS-503 Assignment: Build a Minimal Container Runtime in C

## Goal

In this assignment, you'll write a C program that replicates the core behavior of `docker run nginx`. Your program will use Linux system calls to:

1. Set up process isolation (via namespaces and cgroups).
2. Use `chroot` to simulate a container filesystem.
3. Start the `nginx` binary in a sandboxed environment.
4. Serve HTTP content on port 80.

## What You Will Learn

- Use of `clone()`, `unshare()`, and namespaces for process isolation.
- Basics of `chroot()` and mounting filesystems.
- How containers isolate processes and networking.
- How to execute and supervise a real-world binary (`nginx`) inside a restricted environment.

## Starter Files

- `mini_docker.c` — contains boilerplate code, function stubs, and helper macros.
- You’ll need to install `nginx` inside a minimal rootfs (provided or downloaded).

## Environment Setup

This assignment must be run on a Linux system with root privileges (e.g., WSL2 or a VM).

You can use the provided script to create a minimal `nginx` rootfs:

```
make rootfs
```
