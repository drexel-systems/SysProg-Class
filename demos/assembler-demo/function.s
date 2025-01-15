.data
    prompt: .asciiz "The value of c is "
    new_ln: .asciiz "\n"

.text
    .globl _main

main:
    addi sp, sp, -12         #save space on stack for 3 words (32 bits each)
    sw s0, 0(sp)
    sw s1, 4(sp)
    sw s2, 4(sp)

    #values now saved on stack so we can use these registers

    li s1, 5  # s0=5
    li s2, 10 # s0=5
    
    #to call a function, convention is args are in a0, a1, a2, ...
    mv a0, s1
    mv a1, s2

    jal add_fn  #function call, we need to handle the fact that 
                #execution will resume at the next instruction
                #jal automatically sets ra register to this 
                #address

    #by convention result is in a0, lets stash this in s0 becuase
    #we need a0 for system calls
    mv s0, a0

    #print out result
    li a0, 4    #4 is syscall for print string
    la a1, prompt
    ecall

    #remember sum is now in s0, move it to a1
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


add_fn:     #functions are just labels
    #let setup the stack, we need space for c and to save the return address
    addi sp, sp, -8         #save space on stack for 3 words (32 bits each)
    sw s0, 0(sp)
    sw ra, 4(sp)

    #by convention, args are passed in a0, a1, ...
    #and return values are in a0
    add s0, a0, a1      #lets add the values

    #obviously this is a simple function we can do more here

    #lets setup the return value in a0 by convention
    mv a0, s0

    #now lets restore the stack

    sw s0, 0(sp)
    sw ra, 4(sp)
    addi sp, sp, 8         #save space on stack for 3 words (32 bits each)

    #lets return to where we belong
    jr ra
    


    
     
    