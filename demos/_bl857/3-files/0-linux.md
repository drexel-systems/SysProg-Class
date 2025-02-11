# linux filesystem layout

tree -L 1 / 

/bin
/home
/tmp
/etc
/var    
/dev    # represents physical and virtual devices
/proc   # virtual - a "window into the kernel"

# proc

cpuinfo
meminfo

echo $$
cd /proc/$$
ls -lah
cat environ

# dev

tty                         # open two terms and identify tty
echo "hello" > /dev/pts/1   # send some data from one tty to another

# /dev/pts - "char" device type
ls -lah /dev/pts/
total 0
drwxr-xr-x  2 root root      0 Jan 23 10:56 .
drwxr-xr-x 11 root root   1.1K Jan 23 11:12 ..
crw-------  1 root tty  136, 0 Jan 23 10:56 0
crw--w----  1 ben  tty  136, 1 Jan 27 11:55 1
crw--w----  1 ben  tty  136, 3 Jan 26 09:29 3
crw--w----  1 ben  tty  136, 4 Jan 26 09:29 4
crw--w----  1 ben  tty  136, 5 Jan 27 11:56 5
crw--w----  1 ben  tty  136, 6 Jan 26 14:09 6
crw--w----  1 ben  tty  136, 7 Jan 26 14:09 7
crw--w----  1 ben  tty  136, 8 Jan 27 10:28 8
crw-rw-rw-  1 root tty    5, 2 Jan 27 11:56 ptmx


# pipes

cd demos/file-stream

mkfifo mypipe

watch -n 1 'echo date > mypipe'