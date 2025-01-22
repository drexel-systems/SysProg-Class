	.file	"0-basics.c"
	.text
.Ltext0:
	.file 0 "/home/bnm/SysProg-Class/demos/1-pointers" "0-basics.c"
	.section	.rodata
.LC0:
	.string	"%x\n"
.LC1:
	.string	"hello, world"
.LC2:
	.string	"Memory allocation failed\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.file 1 "0-basics.c"
	.loc 1 11 1
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$48, %rsp
	.loc 1 11 1
	movq	%fs:40, %rax
	movq	%rax, -8(%rbp)
	xorl	%eax, %eax
	.loc 1 14 9
	movl	$100, -32(%rbp)
	movl	$101, -28(%rbp)
	movl	$102, -24(%rbp)
	movl	$103, -20(%rbp)
	.loc 1 17 5
	leaq	-32(%rbp), %rax
	movq	%rax, %rsi
	leaq	.LC0(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	.loc 1 19 5
	leaq	-32(%rbp), %rax
	movl	$4, %esi
	movq	%rax, %rdi
	call	loop_print_ints
	.loc 1 22 11
	leaq	.LC1(%rip), %rax
	movq	%rax, -48(%rbp)
	.loc 1 24 18
	movl	$50, %edi
	call	malloc@PLT
	movq	%rax, -40(%rbp)
	.loc 1 25 8
	cmpq	$0, -40(%rbp)
	jne	.L2
	.loc 1 27 9
	movq	stderr(%rip), %rax
	movq	%rax, %rcx
	movl	$25, %edx
	movl	$1, %esi
	leaq	.LC2(%rip), %rax
	movq	%rax, %rdi
	call	fwrite@PLT
	.loc 1 28 16
	movl	$-1, %eax
	jmp	.L4
.L2:
	.loc 1 31 5
	movq	-48(%rbp), %rdx
	movq	-40(%rbp), %rax
	movq	%rdx, %rsi
	movq	%rax, %rdi
	call	strcpy@PLT
	.loc 1 33 5
	movq	-40(%rbp), %rax
	movl	$50, %esi
	movq	%rax, %rdi
	call	loop_print_string
	.loc 1 35 12
	movl	$0, %eax
.L4:
	.loc 1 36 1
	movq	-8(%rbp), %rdx
	subq	%fs:40, %rdx
	je	.L5
	call	__stack_chk_fail@PLT
.L5:
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
.LC3:
	.string	"%c\n"
	.text
	.globl	loop_print_string
	.type	loop_print_string, @function
loop_print_string:
.LFB7:
	.loc 1 39 1
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
.LBB2:
	.loc 1 40 14
	movl	$0, -4(%rbp)
	.loc 1 40 5
	jmp	.L7
.L10:
	.loc 1 42 13
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	.loc 1 42 12
	testb	%al, %al
	je	.L11
	.loc 1 46 24
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	.loc 1 46 9
	movsbl	%al, %eax
	movl	%eax, %esi
	leaq	.LC3(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	.loc 1 47 17
	addq	$1, -24(%rbp)
	.loc 1 40 37 discriminator 2
	addl	$1, -4(%rbp)
.L7:
	.loc 1 40 23 discriminator 1
	movl	-4(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L10
	jmp	.L6
.L11:
	.loc 1 44 13
	nop
.L6:
.LBE2:
	.loc 1 52 1
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	loop_print_string, .-loop_print_string
	.section	.rodata
.LC4:
	.string	"%d %d\n"
.LC5:
	.string	"%d\n"
	.text
	.globl	loop_print_ints
	.type	loop_print_ints, @function
loop_print_ints:
.LFB8:
	.loc 1 55 1
	.cfi_startproc
	endbr64
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$32, %rsp
	movq	%rdi, -24(%rbp)
	movl	%esi, -28(%rbp)
.LBB3:
	.loc 1 56 14
	movl	$0, -16(%rbp)
	.loc 1 56 5
	jmp	.L13
.L14:
	.loc 1 59 9
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	.loc 1 59 41
	movl	-16(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rcx
	movq	-24(%rbp), %rax
	addq	%rcx, %rax
	.loc 1 59 9
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	.loc 1 62 35
	movl	-16(%rbp), %eax
	cltq
	leaq	0(,%rax,4), %rdx
	movq	-24(%rbp), %rax
	addq	%rdx, %rax
	.loc 1 62 9
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC5(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	.loc 1 56 38 discriminator 3
	addl	$1, -16(%rbp)
.L13:
	.loc 1 56 23 discriminator 1
	movl	-16(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L14
.LBE3:
	.loc 1 76 10
	movq	-24(%rbp), %rax
	movq	%rax, -8(%rbp)
.LBB4:
	.loc 1 78 14
	movl	$0, -12(%rbp)
	.loc 1 78 5
	jmp	.L15
.L16:
	.loc 1 80 9
	movq	-24(%rbp), %rax
	movl	(%rax), %edx
	movq	-8(%rbp), %rax
	movl	(%rax), %eax
	movl	%eax, %esi
	leaq	.LC4(%rip), %rax
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf@PLT
	.loc 1 81 16
	addq	$4, -8(%rbp)
	.loc 1 78 38 discriminator 3
	addl	$1, -12(%rbp)
.L15:
	.loc 1 78 23 discriminator 1
	movl	-12(%rbp), %eax
	cmpl	-28(%rbp), %eax
	jl	.L16
.LBE4:
	.loc 1 83 1
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	loop_print_ints, .-loop_print_ints
.Letext0:
	.file 2 "/usr/include/x86_64-linux-gnu/bits/types.h"
	.file 3 "/usr/include/x86_64-linux-gnu/bits/types/struct_FILE.h"
	.file 4 "/usr/include/x86_64-linux-gnu/bits/types/FILE.h"
	.file 5 "/usr/include/string.h"
	.file 6 "/usr/include/stdlib.h"
	.file 7 "/usr/include/stdio.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.long	0x400
	.value	0x5
	.byte	0x1
	.byte	0x8
	.long	.Ldebug_abbrev0
	.uleb128 0xf
	.long	.LASF61
	.byte	0x1d
	.long	.LASF0
	.long	.LASF1
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.long	.Ldebug_line0
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF2
	.uleb128 0x2
	.byte	0x4
	.byte	0x7
	.long	.LASF3
	.uleb128 0x10
	.byte	0x8
	.uleb128 0x2
	.byte	0x1
	.byte	0x8
	.long	.LASF4
	.uleb128 0x2
	.byte	0x2
	.byte	0x7
	.long	.LASF5
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF6
	.uleb128 0x2
	.byte	0x2
	.byte	0x5
	.long	.LASF7
	.uleb128 0x11
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF8
	.uleb128 0x7
	.long	.LASF9
	.byte	0x2
	.byte	0x98
	.byte	0x19
	.long	0x61
	.uleb128 0x7
	.long	.LASF10
	.byte	0x2
	.byte	0x99
	.byte	0x1b
	.long	0x61
	.uleb128 0x3
	.long	0x85
	.uleb128 0x2
	.byte	0x1
	.byte	0x6
	.long	.LASF11
	.uleb128 0x12
	.long	0x85
	.uleb128 0x13
	.long	.LASF62
	.byte	0xd8
	.byte	0x3
	.byte	0x31
	.byte	0x8
	.long	0x1fb
	.uleb128 0x1
	.long	.LASF12
	.byte	0x33
	.byte	0x7
	.long	0x5a
	.byte	0
	.uleb128 0x1
	.long	.LASF13
	.byte	0x36
	.byte	0x9
	.long	0x80
	.byte	0x8
	.uleb128 0x1
	.long	.LASF14
	.byte	0x37
	.byte	0x9
	.long	0x80
	.byte	0x10
	.uleb128 0x1
	.long	.LASF15
	.byte	0x38
	.byte	0x9
	.long	0x80
	.byte	0x18
	.uleb128 0x1
	.long	.LASF16
	.byte	0x39
	.byte	0x9
	.long	0x80
	.byte	0x20
	.uleb128 0x1
	.long	.LASF17
	.byte	0x3a
	.byte	0x9
	.long	0x80
	.byte	0x28
	.uleb128 0x1
	.long	.LASF18
	.byte	0x3b
	.byte	0x9
	.long	0x80
	.byte	0x30
	.uleb128 0x1
	.long	.LASF19
	.byte	0x3c
	.byte	0x9
	.long	0x80
	.byte	0x38
	.uleb128 0x1
	.long	.LASF20
	.byte	0x3d
	.byte	0x9
	.long	0x80
	.byte	0x40
	.uleb128 0x1
	.long	.LASF21
	.byte	0x40
	.byte	0x9
	.long	0x80
	.byte	0x48
	.uleb128 0x1
	.long	.LASF22
	.byte	0x41
	.byte	0x9
	.long	0x80
	.byte	0x50
	.uleb128 0x1
	.long	.LASF23
	.byte	0x42
	.byte	0x9
	.long	0x80
	.byte	0x58
	.uleb128 0x1
	.long	.LASF24
	.byte	0x44
	.byte	0x16
	.long	0x214
	.byte	0x60
	.uleb128 0x1
	.long	.LASF25
	.byte	0x46
	.byte	0x14
	.long	0x219
	.byte	0x68
	.uleb128 0x1
	.long	.LASF26
	.byte	0x48
	.byte	0x7
	.long	0x5a
	.byte	0x70
	.uleb128 0x1
	.long	.LASF27
	.byte	0x49
	.byte	0x7
	.long	0x5a
	.byte	0x74
	.uleb128 0x1
	.long	.LASF28
	.byte	0x4a
	.byte	0xb
	.long	0x68
	.byte	0x78
	.uleb128 0x1
	.long	.LASF29
	.byte	0x4d
	.byte	0x12
	.long	0x45
	.byte	0x80
	.uleb128 0x1
	.long	.LASF30
	.byte	0x4e
	.byte	0xf
	.long	0x4c
	.byte	0x82
	.uleb128 0x1
	.long	.LASF31
	.byte	0x4f
	.byte	0x8
	.long	0x21e
	.byte	0x83
	.uleb128 0x1
	.long	.LASF32
	.byte	0x51
	.byte	0xf
	.long	0x22e
	.byte	0x88
	.uleb128 0x1
	.long	.LASF33
	.byte	0x59
	.byte	0xd
	.long	0x74
	.byte	0x90
	.uleb128 0x1
	.long	.LASF34
	.byte	0x5b
	.byte	0x17
	.long	0x238
	.byte	0x98
	.uleb128 0x1
	.long	.LASF35
	.byte	0x5c
	.byte	0x19
	.long	0x242
	.byte	0xa0
	.uleb128 0x1
	.long	.LASF36
	.byte	0x5d
	.byte	0x14
	.long	0x219
	.byte	0xa8
	.uleb128 0x1
	.long	.LASF37
	.byte	0x5e
	.byte	0x9
	.long	0x3c
	.byte	0xb0
	.uleb128 0x1
	.long	.LASF38
	.byte	0x5f
	.byte	0x15
	.long	0x247
	.byte	0xb8
	.uleb128 0x1
	.long	.LASF39
	.byte	0x60
	.byte	0x7
	.long	0x5a
	.byte	0xc0
	.uleb128 0x1
	.long	.LASF40
	.byte	0x62
	.byte	0x8
	.long	0x24c
	.byte	0xc4
	.byte	0
	.uleb128 0x7
	.long	.LASF41
	.byte	0x4
	.byte	0x7
	.byte	0x19
	.long	0x91
	.uleb128 0x14
	.long	.LASF63
	.byte	0x3
	.byte	0x2b
	.byte	0xe
	.uleb128 0x8
	.long	.LASF42
	.uleb128 0x3
	.long	0x20f
	.uleb128 0x3
	.long	0x91
	.uleb128 0xb
	.long	0x85
	.long	0x22e
	.uleb128 0x9
	.long	0x2e
	.byte	0
	.byte	0
	.uleb128 0x3
	.long	0x207
	.uleb128 0x8
	.long	.LASF43
	.uleb128 0x3
	.long	0x233
	.uleb128 0x8
	.long	.LASF44
	.uleb128 0x3
	.long	0x23d
	.uleb128 0x3
	.long	0x219
	.uleb128 0xb
	.long	0x85
	.long	0x25c
	.uleb128 0x9
	.long	0x2e
	.byte	0x13
	.byte	0
	.uleb128 0x3
	.long	0x8c
	.uleb128 0x3
	.long	0x1fb
	.uleb128 0x15
	.long	.LASF64
	.byte	0x7
	.byte	0x97
	.byte	0xe
	.long	0x261
	.uleb128 0x2
	.byte	0x8
	.byte	0x5
	.long	.LASF45
	.uleb128 0x2
	.byte	0x8
	.byte	0x7
	.long	.LASF46
	.uleb128 0x16
	.long	.LASF47
	.byte	0x5
	.byte	0x8d
	.byte	0xe
	.long	0x80
	.long	0x29b
	.uleb128 0x4
	.long	0x80
	.uleb128 0x4
	.long	0x25c
	.byte	0
	.uleb128 0xc
	.long	.LASF48
	.byte	0x6
	.value	0x2a0
	.byte	0xe
	.long	0x3c
	.long	0x2b2
	.uleb128 0x4
	.long	0x2e
	.byte	0
	.uleb128 0xc
	.long	.LASF49
	.byte	0x7
	.value	0x16b
	.byte	0xc
	.long	0x5a
	.long	0x2ca
	.uleb128 0x4
	.long	0x25c
	.uleb128 0x17
	.byte	0
	.uleb128 0xd
	.long	.LASF53
	.byte	0x36
	.quad	.LFB8
	.quad	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.long	0x34f
	.uleb128 0x5
	.long	.LASF50
	.byte	0x36
	.byte	0x1b
	.long	0x34f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.long	.LASF51
	.byte	0x36
	.byte	0x2c
	.long	0x5a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0x6
	.long	.LASF52
	.byte	0x4c
	.byte	0xa
	.long	0x34f
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x18
	.quad	.LBB3
	.quad	.LBE3-.LBB3
	.long	0x331
	.uleb128 0xa
	.string	"i"
	.byte	0x38
	.long	0x5a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.byte	0
	.uleb128 0xe
	.quad	.LBB4
	.quad	.LBE4-.LBB4
	.uleb128 0xa
	.string	"i"
	.byte	0x4e
	.long	0x5a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -28
	.byte	0
	.byte	0
	.uleb128 0x3
	.long	0x5a
	.uleb128 0xd
	.long	.LASF54
	.byte	0x26
	.quad	.LFB7
	.quad	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.long	0x3aa
	.uleb128 0x5
	.long	.LASF55
	.byte	0x26
	.byte	0x1e
	.long	0x80
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x5
	.long	.LASF56
	.byte	0x26
	.byte	0x2c
	.long	0x5a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -44
	.uleb128 0xe
	.quad	.LBB2
	.quad	.LBE2-.LBB2
	.uleb128 0xa
	.string	"i"
	.byte	0x28
	.long	0x5a
	.uleb128 0x2
	.byte	0x91
	.sleb128 -20
	.byte	0
	.byte	0
	.uleb128 0x19
	.long	.LASF57
	.byte	0x1
	.byte	0xa
	.byte	0x5
	.long	0x5a
	.quad	.LFB6
	.quad	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.long	0x3f7
	.uleb128 0x6
	.long	.LASF58
	.byte	0xe
	.byte	0x9
	.long	0x3f7
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x6
	.long	.LASF59
	.byte	0x16
	.byte	0xb
	.long	0x80
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x6
	.long	.LASF60
	.byte	0x18
	.byte	0xb
	.long	0x80
	.uleb128 0x2
	.byte	0x91
	.sleb128 -56
	.byte	0
	.uleb128 0x1a
	.long	0x5a
	.uleb128 0x9
	.long	0x2e
	.byte	0x3
	.byte	0
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 3
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x2
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0xe
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x4
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x5
	.uleb128 0x5
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x6
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x7
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0x13
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 14
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xc
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0x5
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 1
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0x21
	.sleb128 6
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0xf
	.uleb128 0x11
	.byte	0x1
	.uleb128 0x25
	.uleb128 0xe
	.uleb128 0x13
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x1f
	.uleb128 0x1b
	.uleb128 0x1f
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x10
	.uleb128 0x17
	.byte	0
	.byte	0
	.uleb128 0x10
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x11
	.uleb128 0x24
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3e
	.uleb128 0xb
	.uleb128 0x3
	.uleb128 0x8
	.byte	0
	.byte	0
	.uleb128 0x12
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x13
	.byte	0x1
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x16
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x34
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x17
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x18
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x19
	.uleb128 0x2e
	.byte	0x1
	.uleb128 0x3f
	.uleb128 0x19
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0xb
	.uleb128 0x3b
	.uleb128 0xb
	.uleb128 0x39
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.uleb128 0x40
	.uleb128 0x18
	.uleb128 0x7c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x1a
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.long	0x2c
	.value	0x2
	.long	.Ldebug_info0
	.byte	0x8
	.byte	0
	.value	0
	.value	0
	.quad	.Ltext0
	.quad	.Letext0-.Ltext0
	.quad	0
	.quad	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF49:
	.string	"printf"
.LASF9:
	.string	"__off_t"
.LASF13:
	.string	"_IO_read_ptr"
.LASF48:
	.string	"malloc"
.LASF25:
	.string	"_chain"
.LASF31:
	.string	"_shortbuf"
.LASF61:
	.string	"GNU C17 14.2.0 -mtune=generic -march=x86-64 -g -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection -fcf-protection"
.LASF19:
	.string	"_IO_buf_base"
.LASF46:
	.string	"long long unsigned int"
.LASF34:
	.string	"_codecvt"
.LASF45:
	.string	"long long int"
.LASF6:
	.string	"signed char"
.LASF60:
	.string	"buff"
.LASF26:
	.string	"_fileno"
.LASF14:
	.string	"_IO_read_end"
.LASF8:
	.string	"long int"
.LASF51:
	.string	"array_size"
.LASF55:
	.string	"str_buff"
.LASF12:
	.string	"_flags"
.LASF20:
	.string	"_IO_buf_end"
.LASF29:
	.string	"_cur_column"
.LASF43:
	.string	"_IO_codecvt"
.LASF28:
	.string	"_old_offset"
.LASF33:
	.string	"_offset"
.LASF42:
	.string	"_IO_marker"
.LASF50:
	.string	"input_array"
.LASF3:
	.string	"unsigned int"
.LASF37:
	.string	"_freeres_buf"
.LASF59:
	.string	"mystr"
.LASF2:
	.string	"long unsigned int"
.LASF17:
	.string	"_IO_write_ptr"
.LASF53:
	.string	"loop_print_ints"
.LASF5:
	.string	"short unsigned int"
.LASF56:
	.string	"buff_size"
.LASF21:
	.string	"_IO_save_base"
.LASF47:
	.string	"strcpy"
.LASF32:
	.string	"_lock"
.LASF27:
	.string	"_flags2"
.LASF39:
	.string	"_mode"
.LASF18:
	.string	"_IO_write_end"
.LASF38:
	.string	"_prevchain"
.LASF63:
	.string	"_IO_lock_t"
.LASF62:
	.string	"_IO_FILE"
.LASF24:
	.string	"_markers"
.LASF4:
	.string	"unsigned char"
.LASF52:
	.string	"p_myptr"
.LASF7:
	.string	"short int"
.LASF44:
	.string	"_IO_wide_data"
.LASF30:
	.string	"_vtable_offset"
.LASF41:
	.string	"FILE"
.LASF11:
	.string	"char"
.LASF10:
	.string	"__off64_t"
.LASF15:
	.string	"_IO_read_base"
.LASF58:
	.string	"digits"
.LASF23:
	.string	"_IO_save_end"
.LASF40:
	.string	"_unused2"
.LASF64:
	.string	"stderr"
.LASF54:
	.string	"loop_print_string"
.LASF22:
	.string	"_IO_backup_base"
.LASF36:
	.string	"_freeres_list"
.LASF35:
	.string	"_wide_data"
.LASF57:
	.string	"main"
.LASF16:
	.string	"_IO_write_base"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"0-basics.c"
.LASF1:
	.string	"/home/bnm/SysProg-Class/demos/1-pointers"
	.ident	"GCC: (Ubuntu 14.2.0-4ubuntu2) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
	.section	.note.gnu.property,"a"
	.align 8
	.long	1f - 0f
	.long	4f - 1f
	.long	5
0:
	.string	"GNU"
1:
	.align 8
	.long	0xc0000002
	.long	3f - 2f
2:
	.long	0x3
3:
	.align 8
4:
