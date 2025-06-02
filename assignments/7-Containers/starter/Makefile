CC = gcc
CFLAGS = -Wall -Wextra -O2
TARGET = mini_docker
SRC = mini_docker.c

.PHONY: all clean rootfs run

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

rootfs:
	@echo "🏗️  Creating nginx root filesystem..."
	sudo bash make_nginx_rootfs.sh

run: $(TARGET)
	sudo ./$(TARGET)

clean:
	rm -f $(TARGET)
