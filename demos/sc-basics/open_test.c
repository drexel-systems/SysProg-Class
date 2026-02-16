#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>


int main(){
    int fd1 = open("scratch_pad", O_WRONLY, 0644);
    int fd2 = open("basic-write.c",0);
    int fd3 = open("does_not_exit.c", 0);

    printf("open fd1 = %d\n",fd1);
    printf("open fd2 = %d\n",fd2);
    printf("open f3 = %d\n",fd3);

    int rc = write(fd1, "Hello there\n", 12);
    int rc1 = write(1, "Hello there\n", 12);
    printf("write rc = %d, write rc1 = %d\n",rc, rc1);

}