.data
    a:      .word 5
    b:      .word 10
    c:      .word 0
    prompt: .asciiz "The value of c is "
    new_ln: .asciiz "\n"

.text
    .globl _main

main:
    la t1, a 
    lw t1, 0(t1)

    la t2, b 
    lw t2, 0(t2)

    add t3, t1, t2
    la t4, c
    sw t3, 0(t4)

    li a0, 4    #4 is syscall for print string
    la a1, prompt
    ecall

    li a0, 1    #1 is syscall for print integer
    la a1, c
    lw a1, 0(a1)
    ecall 

    li a0, 4    #4 is syscall for print string
    la a1, new_ln
    ecall

    li a0, 10   # 10 is syscall for exit
    ecall

    


    
     
    