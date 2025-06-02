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
	@echo "🧹 Cleaning build artifacts and rootfs..."

	@rm -f $(TARGET)

	@if [ -d "$(ROOTFS_DIR)" ]; then \
		echo "🔌 Attempting to unmount nested filesystems..."; \
		sudo fuser -km $(ROOTFS_DIR) || true; \
		for dir in proc dev/pts dev sys; do \
			sudo umount -lf $(ROOTFS_DIR)/$$dir 2>/dev/null || true; \
		done; \
		sudo umount -lf $(ROOTFS_DIR) 2>/dev/null || true; \
		echo "🗑️  Removing rootfs..."; \
		sudo rm -rf $(ROOTFS_DIR); \
	fi