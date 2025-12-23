CC = gcc
CFLAGS = -Wall -Wextra -g -std=c11
TARGET = wordcount

all: $(TARGET)

$(TARGET): wordcount.c
	$(CC) $(CFLAGS) -o $(TARGET) wordcount.c

clean:
	rm -f $(TARGET)

.PHONY: all clean