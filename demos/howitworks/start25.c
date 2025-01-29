//compile:  gcc -nostartfiles -o start25 start25.c 
//    run:  ./start25; echo $?
//should produce 22 in shell


//Discussion points, this program starts but never returns
//lets discuss:
//   1. What is _start(), where is main()?
//   2. Why does it not return

extern void _exit(int);

void _start() {
    _exit(25);
}