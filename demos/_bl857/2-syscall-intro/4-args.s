	.arch armv8-a
	.file	"4-args.c"
	.text
.Ltext0:
	.file 0 "/home/ben/SysProg-Class/demos/_bl857/2-syscall-intro" "4-args.c"
	.align	2
	.global	main
	.type	main, %function
main:
.LFB6:
	.file 1 "4-args.c"
	.loc 1 19 1
	.cfi_startproc
	stp	x29, x30, [sp, -16]!
	.cfi_def_cfa_offset 16
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	mov	x29, sp
	.loc 1 20 5
	bl	simple_with_args
	.loc 1 21 5
	bl	simple_with_struct
	.loc 1 23 12
	mov	w0, 0
	.loc 1 24 1
	ldp	x29, x30, [sp], 16
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align	3
.LC0:
	.string	"Hello, simple_with_args world!\n"
	.align	3
.LC1:
	.string	"Failed to write\n"
	.text
	.align	2
	.global	simple_with_args
	.type	simple_with_args, %function
simple_with_args:
.LFB7:
	.loc 1 27 1
	.cfi_startproc
	stp	x29, x30, [sp, -48]!
	.cfi_def_cfa_offset 48
	.cfi_offset 29, -48
	.cfi_offset 30, -40
	mov	x29, sp
	.loc 1 28 17
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	str	x0, [sp, 16]
	.loc 1 29 21
	ldr	x0, [sp, 16]
	bl	strlen
	str	x0, [sp, 24]
	.loc 1 34 29
	ldr	x3, [sp, 24]
	ldr	x2, [sp, 16]
	mov	w1, 1
	mov	x0, 64
	bl	syscall
	str	x0, [sp, 32]
	.loc 1 37 8
	ldr	x0, [sp, 32]
	cmn	x0, #1
	bne	.L5
.LBB2:
	.loc 1 39 21
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	str	x0, [sp, 40]
	.loc 1 40 9
	mov	w3, 15
	ldr	x2, [sp, 40]
	mov	w1, 2
	mov	x0, 64
	bl	syscall
.L5:
.LBE2:
	.loc 1 42 1
	nop
	ldp	x29, x30, [sp], 48
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE7:
	.size	simple_with_args, .-simple_with_args
	.section	.rodata
	.align	3
.LC2:
	.string	"Failed to allocate stack"
	.align	3
.LC3:
	.string	"syscall clone3 failed"
	.align	3
.LC4:
	.string	"Hello simple_with_struct world!"
	.align	3
.LC5:
	.string	"Child process created with PID: %d\n"
	.text
	.align	2
	.global	simple_with_struct
	.type	simple_with_struct, %function
simple_with_struct:
.LFB8:
	.loc 1 45 1
	.cfi_startproc
	sub	sp, sp, #144
	.cfi_def_cfa_offset 144
	stp	x29, x30, [sp, 128]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 128
	.loc 1 45 1
	adrp	x0, :got:__stack_chk_guard;ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 120]
	mov	x1, 0
	.loc 1 83 5
	add	x0, sp, 32
	mov	x2, 88
	mov	w1, 0
	bl	memset
	.loc 1 85 16
	mov	x0, 768
	str	x0, [sp, 32]
	.loc 1 86 22
	mov	x0, 17
	str	x0, [sp, 64]
	.loc 1 89 12
	mov	x0, 65536
	str	x0, [sp, 16]
	.loc 1 90 19
	ldr	x0, [sp, 16]
	bl	malloc
	str	x0, [sp, 24]
	.loc 1 91 8
	ldr	x0, [sp, 24]
	cmp	x0, 0
	bne	.L7
	.loc 1 93 9
	adrp	x0, .LC2
	add	x0, x0, :lo12:.LC2
	bl	perror
	.loc 1 94 9
	mov	w0, -1
	bl	_exit
.L7:
	.loc 1 98 40
	ldr	x1, [sp, 24]
	ldr	x0, [sp, 16]
	add	x0, x1, x0
	.loc 1 98 16
	str	x0, [sp, 72]
	.loc 1 99 21
	ldr	x0, [sp, 16]
	str	x0, [sp, 80]
	.loc 1 102 18
	add	x0, sp, 32
	mov	x2, 88
	mov	x1, x0
	mov	x0, 435
	bl	syscall
	.loc 1 102 9 discriminator 1
	str	w0, [sp, 12]
	.loc 1 104 8
	ldr	w0, [sp, 12]
	cmn	w0, #1
	bne	.L8
	.loc 1 106 9
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	perror
	.loc 1 107 9
	ldr	x0, [sp, 24]
	bl	free
	.loc 1 108 9
	mov	w0, -1
	bl	_exit
.L8:
	.loc 1 111 8
	ldr	w0, [sp, 12]
	cmp	w0, 0
	bne	.L9
	.loc 1 114 9
	adrp	x0, .LC4
	add	x0, x0, :lo12:.LC4
	bl	puts
	.loc 1 115 9
	mov	w0, 0
	bl	_exit
.L9:
	.loc 1 120 9
	ldr	w1, [sp, 12]
	adrp	x0, .LC5
	add	x0, x0, :lo12:.LC5
	bl	printf
	.loc 1 124 5
	ldr	x0, [sp, 24]
	bl	free
	.loc 1 125 1
	nop
	adrp	x0, :got:__stack_chk_guard;ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x2, [sp, 120]
	ldr	x1, [x0]
	subs	x2, x2, x1
	mov	x1, 0
	beq	.L10
	bl	__stack_chk_fail
.L10:
	ldp	x29, x30, [sp, 128]
	add	sp, sp, 144
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE8:
	.size	simple_with_struct, .-simple_with_struct
.Letext0:
	.file 2 "/usr/include/aarch64-linux-gnu/bits/types.h"
	.file 3 "/usr/include/unistd.h"
	.file 4 "/usr/lib/gcc/aarch64-linux-gnu/14/include/stddef.h"
	.file 5 "/usr/include/asm-generic/int-ll64.h"
	.file 6 "/usr/include/aarch64-linux-gnu/bits/stdint-uintn.h"
	.file 7 "/usr/include/stdio.h"
	.file 8 "/usr/include/stdlib.h"
	.file 9 "/usr/include/string.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x329
	.2byte	0x5
	.byte	0x1
	.byte	0x8
	.4byte	.Ldebug_abbrev0
	.uleb128 0xa
	.4byte	.LASF48
	.byte	0x1d
	.4byte	.LASF0
	.4byte	.LASF1
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF2
	.uleb128 0x1
	.byte	0x2
	.byte	0x7
	.4byte	.LASF3
	.uleb128 0x1
	.byte	0x4
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x1
	.byte	0x1
	.byte	0x6
	.4byte	.LASF6
	.uleb128 0x4
	.4byte	.LASF9
	.byte	0x2
	.byte	0x26
	.byte	0x17
	.4byte	0x2e
	.uleb128 0x1
	.byte	0x2
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0xb
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF8
	.uleb128 0x4
	.4byte	.LASF10
	.byte	0x2
	.byte	0x2d
	.byte	0x1b
	.4byte	0x43
	.uleb128 0xc
	.byte	0x8
	.uleb128 0x4
	.4byte	.LASF11
	.byte	0x2
	.byte	0xc2
	.byte	0x1b
	.4byte	0x6b
	.uleb128 0x1
	.byte	0x1
	.byte	0x8
	.4byte	.LASF12
	.uleb128 0xd
	.4byte	0x8c
	.uleb128 0x4
	.4byte	.LASF13
	.byte	0x3
	.byte	0xdc
	.byte	0x13
	.4byte	0x80
	.uleb128 0x4
	.4byte	.LASF14
	.byte	0x4
	.byte	0xd6
	.byte	0x17
	.4byte	0x43
	.uleb128 0xe
	.byte	0x8
	.4byte	0x93
	.uleb128 0x1
	.byte	0x8
	.byte	0x5
	.4byte	.LASF15
	.uleb128 0x4
	.4byte	.LASF16
	.byte	0x5
	.byte	0x1f
	.byte	0x2a
	.4byte	0xc9
	.uleb128 0x1
	.byte	0x8
	.byte	0x7
	.4byte	.LASF17
	.uleb128 0x1
	.byte	0x10
	.byte	0x5
	.4byte	.LASF18
	.uleb128 0x1
	.byte	0x10
	.byte	0x7
	.4byte	.LASF19
	.uleb128 0x4
	.4byte	.LASF20
	.byte	0x6
	.byte	0x18
	.byte	0x13
	.4byte	0x51
	.uleb128 0x4
	.4byte	.LASF21
	.byte	0x6
	.byte	0x1b
	.byte	0x14
	.4byte	0x72
	.uleb128 0x6
	.4byte	.LASF22
	.byte	0x7
	.2byte	0x16b
	.byte	0xc
	.4byte	0x64
	.4byte	0x10e
	.uleb128 0x2
	.4byte	0xb0
	.uleb128 0x7
	.byte	0
	.uleb128 0x8
	.4byte	.LASF24
	.byte	0x8
	.2byte	0x2af
	.4byte	0x120
	.uleb128 0x2
	.4byte	0x7e
	.byte	0
	.uleb128 0xf
	.4byte	.LASF23
	.byte	0x3
	.2byte	0x270
	.byte	0xd
	.4byte	0x133
	.uleb128 0x2
	.4byte	0x64
	.byte	0
	.uleb128 0x8
	.4byte	.LASF25
	.byte	0x7
	.2byte	0x364
	.4byte	0x145
	.uleb128 0x2
	.4byte	0xb0
	.byte	0
	.uleb128 0x6
	.4byte	.LASF26
	.byte	0x8
	.2byte	0x2a0
	.byte	0xe
	.4byte	0x7e
	.4byte	0x15c
	.uleb128 0x2
	.4byte	0x43
	.byte	0
	.uleb128 0x10
	.4byte	.LASF27
	.byte	0x9
	.byte	0x3d
	.byte	0xe
	.4byte	0x7e
	.4byte	0x17c
	.uleb128 0x2
	.4byte	0x7e
	.uleb128 0x2
	.4byte	0x64
	.uleb128 0x2
	.4byte	0x43
	.byte	0
	.uleb128 0x6
	.4byte	.LASF28
	.byte	0x3
	.2byte	0x443
	.byte	0x11
	.4byte	0x6b
	.4byte	0x194
	.uleb128 0x2
	.4byte	0x6b
	.uleb128 0x7
	.byte	0
	.uleb128 0x6
	.4byte	.LASF29
	.byte	0x9
	.2byte	0x197
	.byte	0xf
	.4byte	0xa4
	.4byte	0x1ab
	.uleb128 0x2
	.4byte	0xb0
	.byte	0
	.uleb128 0x9
	.4byte	.LASF42
	.byte	0x2c
	.8byte	.LFB8
	.8byte	.LFE8-.LFB8
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x297
	.uleb128 0x11
	.4byte	.LASF49
	.byte	0x58
	.byte	0x1
	.byte	0x32
	.byte	0xc
	.4byte	0x25a
	.uleb128 0x3
	.4byte	.LASF30
	.byte	0x34
	.byte	0x12
	.4byte	0xea
	.byte	0
	.uleb128 0x3
	.4byte	.LASF31
	.byte	0x35
	.byte	0x12
	.4byte	0xea
	.byte	0x8
	.uleb128 0x3
	.4byte	.LASF32
	.byte	0x36
	.byte	0x12
	.4byte	0xea
	.byte	0x10
	.uleb128 0x3
	.4byte	.LASF33
	.byte	0x37
	.byte	0x12
	.4byte	0xea
	.byte	0x18
	.uleb128 0x3
	.4byte	.LASF34
	.byte	0x38
	.byte	0x12
	.4byte	0xea
	.byte	0x20
	.uleb128 0x3
	.4byte	.LASF35
	.byte	0x39
	.byte	0x12
	.4byte	0xea
	.byte	0x28
	.uleb128 0x3
	.4byte	.LASF36
	.byte	0x3a
	.byte	0x12
	.4byte	0xea
	.byte	0x30
	.uleb128 0x12
	.string	"tls"
	.byte	0x1
	.byte	0x3b
	.byte	0x12
	.4byte	0xea
	.byte	0x38
	.uleb128 0x3
	.4byte	.LASF37
	.byte	0x3c
	.byte	0x12
	.4byte	0xea
	.byte	0x40
	.uleb128 0x3
	.4byte	.LASF38
	.byte	0x3d
	.byte	0x12
	.4byte	0xea
	.byte	0x48
	.uleb128 0x3
	.4byte	.LASF39
	.byte	0x40
	.byte	0x11
	.4byte	0x297
	.byte	0x50
	.byte	0
	.uleb128 0x5
	.4byte	.LASF40
	.byte	0x52
	.byte	0x17
	.4byte	0x1c7
	.uleb128 0x3
	.byte	0x91
	.sleb128 -112
	.uleb128 0x5
	.4byte	.LASF36
	.byte	0x59
	.byte	0xc
	.4byte	0xa4
	.uleb128 0x3
	.byte	0x91
	.sleb128 -128
	.uleb128 0x5
	.4byte	.LASF35
	.byte	0x5a
	.byte	0xb
	.4byte	0x7e
	.uleb128 0x3
	.byte	0x91
	.sleb128 -120
	.uleb128 0x5
	.4byte	.LASF41
	.byte	0x66
	.byte	0x9
	.4byte	0x64
	.uleb128 0x3
	.byte	0x91
	.sleb128 -132
	.byte	0
	.uleb128 0x13
	.4byte	0xde
	.4byte	0x2a7
	.uleb128 0x14
	.4byte	0x43
	.byte	0
	.byte	0
	.uleb128 0x9
	.4byte	.LASF43
	.byte	0x1a
	.8byte	.LFB7
	.8byte	.LFE7-.LFB7
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x30e
	.uleb128 0x5
	.4byte	.LASF44
	.byte	0x1c
	.byte	0x11
	.4byte	0xb0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x5
	.4byte	.LASF45
	.byte	0x1d
	.byte	0xc
	.4byte	0xa4
	.uleb128 0x2
	.byte	0x91
	.sleb128 -24
	.uleb128 0x5
	.4byte	.LASF46
	.byte	0x22
	.byte	0xd
	.4byte	0x98
	.uleb128 0x2
	.byte	0x91
	.sleb128 -16
	.uleb128 0x15
	.8byte	.LBB2
	.8byte	.LBE2-.LBB2
	.uleb128 0x5
	.4byte	.LASF47
	.byte	0x27
	.byte	0x15
	.4byte	0xb0
	.uleb128 0x2
	.byte	0x91
	.sleb128 -8
	.byte	0
	.byte	0
	.uleb128 0x16
	.4byte	.LASF50
	.byte	0x1
	.byte	0x12
	.byte	0x5
	.4byte	0x64
	.8byte	.LFB6
	.8byte	.LFE6-.LFB6
	.uleb128 0x1
	.byte	0x9c
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
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
	.uleb128 0x2
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x3
	.uleb128 0xd
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
	.uleb128 0x38
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x4
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
	.uleb128 0x5
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
	.uleb128 0x6
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
	.uleb128 0x7
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x8
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
	.uleb128 0x21
	.sleb128 13
	.uleb128 0x27
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x9
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
	.uleb128 0xa
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
	.uleb128 0xb
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
	.uleb128 0xc
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xd
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xe
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xf
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
	.uleb128 0x87
	.uleb128 0x19
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
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
	.uleb128 0x11
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
	.uleb128 0x12
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0x8
	.uleb128 0x3a
	.uleb128 0xb
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
	.uleb128 0x13
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x14
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x16
	.uleb128 0x2e
	.byte	0
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
	.byte	0
	.byte	0
	.byte	0
	.section	.debug_aranges,"",@progbits
	.4byte	0x2c
	.2byte	0x2
	.4byte	.Ldebug_info0
	.byte	0x8
	.byte	0
	.2byte	0
	.2byte	0
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.8byte	0
	.8byte	0
	.section	.debug_line,"",@progbits
.Ldebug_line0:
	.section	.debug_str,"MS",@progbits,1
.LASF15:
	.string	"long long int"
.LASF19:
	.string	"__int128 unsigned"
.LASF9:
	.string	"__uint8_t"
.LASF14:
	.string	"size_t"
.LASF37:
	.string	"set_tid"
.LASF39:
	.string	"reserved"
.LASF34:
	.string	"exit_signal"
.LASF35:
	.string	"stack"
.LASF43:
	.string	"simple_with_args"
.LASF44:
	.string	"message"
.LASF25:
	.string	"perror"
.LASF13:
	.string	"ssize_t"
.LASF29:
	.string	"strlen"
.LASF17:
	.string	"long long unsigned int"
.LASF2:
	.string	"unsigned char"
.LASF48:
	.string	"GNU C17 14.2.0 -mlittle-endian -mabi=lp64 -g -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection"
.LASF46:
	.string	"bytes_written"
.LASF5:
	.string	"long unsigned int"
.LASF3:
	.string	"short unsigned int"
.LASF18:
	.string	"__int128"
.LASF23:
	.string	"_exit"
.LASF27:
	.string	"memset"
.LASF31:
	.string	"pidfd"
.LASF50:
	.string	"main"
.LASF21:
	.string	"uint64_t"
.LASF26:
	.string	"malloc"
.LASF4:
	.string	"unsigned int"
.LASF30:
	.string	"flags"
.LASF28:
	.string	"syscall"
.LASF12:
	.string	"char"
.LASF20:
	.string	"uint8_t"
.LASF41:
	.string	"result"
.LASF11:
	.string	"__ssize_t"
.LASF16:
	.string	"__u64"
.LASF24:
	.string	"free"
.LASF47:
	.string	"error_msg"
.LASF36:
	.string	"stack_size"
.LASF33:
	.string	"parent_tid"
.LASF22:
	.string	"printf"
.LASF49:
	.string	"clone_args"
.LASF7:
	.string	"short int"
.LASF40:
	.string	"args"
.LASF10:
	.string	"__uint64_t"
.LASF38:
	.string	"set_tid_size"
.LASF42:
	.string	"simple_with_struct"
.LASF8:
	.string	"long int"
.LASF6:
	.string	"signed char"
.LASF45:
	.string	"length"
.LASF32:
	.string	"child_tid"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"4-args.c"
.LASF1:
	.string	"/home/ben/SysProg-Class/demos/_bl857/2-syscall-intro"
	.ident	"GCC: (Ubuntu 14.2.0-4ubuntu2) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
