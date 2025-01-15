.data
    prompt: .asciiz "The value of c is "
    new_ln: .asciiz "\n"

.text
    .globl _main

main:
#we are going to use registers s0 for a, s1 for b, and s2 for c
#but since registers are global to the processor we need to save
#the values on the stack before messing with them and restore them
#when we are done

    addi sp, sp, -12         #save space on stack for 3 words (32 bits each)
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 8(sp)

    #values now saved on stack so we can use these registers

    li s1, 5  # s0=5
    li s2, 10 # s0=5
    add s0, s1, s2   #s0 = s1 + s2

    li a0, 4    #4 is syscall for print string
    la a1, prompt
    ecall

    li a0, 1    #1 is syscall for print integer
    mv a1, s0   #sum must be in register a1 for syscall
    ecall 

    li a0, 4    #4 is syscall for print string
    la a1, new_ln
    ecall


    ##################################################
    # NOW LETS RESTORE our registers and pop the stack
    ##################################################
    
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    addi sp, sp, 12   #restore stack space          

    li a0, 10   # 10 is syscall for exit
    ecall

    


    
     
    