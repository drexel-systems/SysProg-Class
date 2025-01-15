.data
    lt_prompt: .asciiz "The value of a < b\n"
    ge_prompt: .asciiz "The value of a >= b\n"

.text
    .globl _main

main:
#we are going to use registers s0 for a, s1 for b, and s2 for c
#but since registers are global to the processor we need to save
#the values on the stack before messing with them and restore them
#when we are done

    addi sp, sp, -8         #save space on stack for 3 words (32 bits each)
    sw s1, 0(sp)
    sw s2, 4(sp)

    #values now saved on stack so we can use these registers

    li s1, 5  # s0=5
    li s2, 10 # s0=5
    
    if_start:       #this is a label
        bge s1, s2, else_cond # if s1 >s2 goto else_cond
        # execution continues naturally when s1<s2
        li a0, 4    #4 is syscall for print string
        la a1, lt_prompt
        ecall
        j done_if  # jump to done_if

    else_cond:
        li a0, 4    #4 is syscall for print string
        la a1, ge_prompt
        ecall

    done_if:
        #we are now done with the if
        
    ##################################################
    # NOW LETS RESTORE our registers and pop the stack
    ##################################################
    
    lw s1, 0(sp)
    lw s2, 4(sp)
    addi sp, sp, 8   #restore stack space          

    li a0, 10   # 10 is syscall for exit
    ecall

    


    
     
    