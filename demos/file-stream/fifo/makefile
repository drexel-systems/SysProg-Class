CC = gcc
CFLAGS = -Wall -Wextra -g

all: pipe-reader pipe-writer

pipe-reader: pipe-reader.c
	$(CC) $(CFLAGS) -o $@ $<

pipe-writer: pipe-writer.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f pipe-reader pipe-writer