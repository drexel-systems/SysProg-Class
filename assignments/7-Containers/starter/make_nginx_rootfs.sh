#!/bin/bash
set -e

ROOTFS_DIR="nginx-rootfs"

echo "🔧 Creating nginx root filesystem at ./$ROOTFS_DIR"

# 1. Create directory
mkdir -p "$ROOTFS_DIR"

# 2. Bootstrap minimal Debian/Ubuntu rootfs
if ! command -v debootstrap &> /dev/null; then
  echo "⚠️  debootstrap not found. Install it with: sudo apt install debootstrap"
  exit 1
fi

sudo debootstrap --variant=minbase focal "$ROOTFS_DIR" http://archive.ubuntu.com/ubuntu

# 3. Mount special filesystems
sudo mount --bind /dev "$ROOTFS_DIR/dev"
sudo mount --bind /dev/pts "$ROOTFS_DIR/dev/pts"
sudo mount --bind /proc "$ROOTFS_DIR/proc"
sudo mount --bind /sys "$ROOTFS_DIR/sys"

# 4. Install nginx inside the chroot
sudo chroot "$ROOTFS_DIR" bash -c "
  apt-get update &&
  DEBIAN_FRONTEND=noninteractive apt-get install -y nginx &&
  apt-get clean
"

# 5. Unmount special filesystems
sudo umount "$ROOTFS_DIR/dev/pts"
sudo umount "$ROOTFS_DIR/dev"
sudo umount "$ROOTFS_DIR/proc"
sudo umount "$ROOTFS_DIR/sys"

# 6. Set up default nginx index page
echo "Hello from your CS503 container!" | sudo tee "$ROOTFS_DIR/var/www/html/index.html" > /dev/null

# 7. Make sure nginx is set to run in foreground
sudo sed -i 's/daemon on;/daemon off;/' "$ROOTFS_DIR/etc/nginx/nginx.conf"

echo "✅ Root filesystem created successfully at $ROOTFS_DIR"
