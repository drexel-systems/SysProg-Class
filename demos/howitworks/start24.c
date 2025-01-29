//compile:  gcc -nostartfiles -o start24 start24.c 
//    run:  ./start24; echo $?
//should produce 22 in shell


//Discussion points, this program starts but never returns
//lets discuss:
//   1. What is _start(), where is main()?
//   2. Why does it not return
int _start() {
    return 24;
}