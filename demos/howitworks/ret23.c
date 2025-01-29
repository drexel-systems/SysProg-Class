//compile:  gcc -o r23 ret23.c
//    run:  ./r23; echo $?
//should produce 22 in shell

//#include <stdlib.h>

//Discussion points
//   run readelf -h ./r23
//       readelf -s ./r23
//       objdump -d ./r23

//Comment out the below, the program will still compile
//and run but we get a warning, lets discuss why

extern void _exit(int);
//extern int puts(const char *);

int main(){
    //under the covers _exit() is providing a status
    //code to the shell after it exits the process
    //no cleanup code is called
    _exit(23);
}