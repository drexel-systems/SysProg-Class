CC = gcc
CFLAGS = -Wall -Wextra -g

all: file-read-async

file-cp-sc: file-cp-sc.c
	$(CC) $(CFLAGS) -o $@ $<

file-cp-libc: file-cp-libc.c
	$(CC) $(CFLAGS) -o $@ $<

file-cp-memmap: file-cp-memmap.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f file-cp-sc file-cp-libc file-cp-memmap