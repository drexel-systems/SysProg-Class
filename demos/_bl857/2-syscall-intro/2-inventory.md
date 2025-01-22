## Concept 1 - this prints what is actually available 

ausyscall --dump

ausyscall --dump | grep getpid

## Concept 2 - "generic" header file

code /usr/include/asm-generic/unistd.h

## Concept 3 - arch-specific header file

code /usr/include/x86_64-linux-gnu/asm/unistd_64.h