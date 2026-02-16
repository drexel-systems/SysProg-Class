#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>

//stand alone program to just print the process id of this 
//program when it runs
int main() {
    long tid = syscall(SYS_gettid);
    int pid = getpid();

    printf("[c] my-pid: My process id is: %d\n", pid);
    printf("[c] my-pid: My thread id is: %ld\n", tid);
    printf("[c] my-pid: Note in a mult-threaded program if pid != tid\n");
    
    return 0;
}