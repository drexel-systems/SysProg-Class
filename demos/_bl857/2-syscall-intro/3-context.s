	.arch armv8-a
	.file	"3-context.c"
	.text
.Ltext0:
	.file 0 "/home/ben/SysProg-Class/demos/_bl857/2-syscall-intro" "3-context.c"
	.section	.rodata
	.align	3
.LC0:
	.string	"fork"
	.align	3
.LC1:
	.string	"c"
	.align	3
.LC2:
	.string	"p"
	.align	3
.LC3:
	.string	"Elapsed time for %d context switches: %.6f seconds\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.file 1 "3-context.c"
	.loc 1 12 12
	.cfi_startproc
	sub	sp, sp, #112
	.cfi_def_cfa_offset 112
	stp	x29, x30, [sp, 96]
	.cfi_offset 29, -16
	.cfi_offset 30, -8
	add	x29, sp, 96
	.loc 1 12 12
	adrp	x0, :got:__stack_chk_guard;ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x1, [x0]
	str	x1, [sp, 88]
	mov	x1, 0
	.loc 1 14 5
	add	x0, sp, 64
	bl	pipe
	.loc 1 15 5
	add	x0, sp, 72
	bl	pipe
	.loc 1 17 17
	bl	fork
	str	w0, [sp, 20]
	.loc 1 18 8
	ldr	w0, [sp, 20]
	cmn	w0, #1
	bne	.L2
	.loc 1 19 9
	adrp	x0, .LC0
	add	x0, x0, :lo12:.LC0
	bl	perror
	.loc 1 20 16
	mov	w0, 1
	b	.L10
.L2:
	.loc 1 23 8
	ldr	w0, [sp, 20]
	cmp	w0, 0
	bne	.L4
.LBB2:
.LBB3:
	.loc 1 26 18
	str	wzr, [sp, 12]
	.loc 1 26 9
	b	.L5
.L6:
	.loc 1 27 13
	ldr	w0, [sp, 64]
	add	x1, sp, 80
	mov	x2, 1
	bl	read
	.loc 1 28 13
	ldr	w3, [sp, 76]
	mov	x2, 1
	adrp	x0, .LC1
	add	x1, x0, :lo12:.LC1
	mov	w0, w3
	bl	write
	.loc 1 26 42 discriminator 3
	ldr	w0, [sp, 12]
	add	w0, w0, 1
	str	w0, [sp, 12]
.L5:
	.loc 1 26 27 discriminator 1
	ldr	w1, [sp, 12]
	mov	w0, 16959
	movk	w0, 0xf, lsl 16
	cmp	w1, w0
	ble	.L6
	b	.L7
.L4:
.LBE3:
.LBE2:
.LBB4:
	.loc 1 36 9
	add	x0, sp, 32
	mov	x1, x0
	mov	w0, 1
	bl	clock_gettime
.LBB5:
	.loc 1 38 18
	str	wzr, [sp, 16]
	.loc 1 38 9
	b	.L8
.L9:
	.loc 1 39 13
	ldr	w3, [sp, 68]
	mov	x2, 1
	adrp	x0, .LC2
	add	x1, x0, :lo12:.LC2
	mov	w0, w3
	bl	write
	.loc 1 40 13
	ldr	w0, [sp, 72]
	add	x1, sp, 80
	mov	x2, 1
	bl	read
	.loc 1 38 42 discriminator 3
	ldr	w0, [sp, 16]
	add	w0, w0, 1
	str	w0, [sp, 16]
.L8:
	.loc 1 38 27 discriminator 1
	ldr	w1, [sp, 16]
	mov	w0, 16959
	movk	w0, 0xf, lsl 16
	cmp	w1, w0
	ble	.L9
.LBE5:
	.loc 1 44 9
	add	x0, sp, 48
	mov	x1, x0
	mov	w0, 1
	bl	clock_gettime
	.loc 1 47 30
	ldr	x1, [sp, 48]
	.loc 1 47 45
	ldr	x0, [sp, 32]
	.loc 1 47 38
	sub	x0, x1, x0
	fmov	d31, x0
	.loc 1 47 54
	scvtf	d30, d31
	.loc 1 48 30
	ldr	x1, [sp, 56]
	.loc 1 48 46
	ldr	x0, [sp, 40]
	.loc 1 48 39
	sub	x0, x1, x0
	fmov	d31, x0
	.loc 1 48 56
	scvtf	d31, d31
	mov	x0, 225833675390976
	movk	x0, 0x41cd, lsl 48
	fmov	d29, x0
	fdiv	d31, d31, d29
	.loc 1 47 16
	fadd	d31, d30, d31
	str	d31, [sp, 24]
	.loc 1 49 9
	ldr	d0, [sp, 24]
	mov	w1, 16960
	movk	w1, 0xf, lsl 16
	adrp	x0, .LC3
	add	x0, x0, :lo12:.LC3
	bl	printf
	.loc 1 52 9
	mov	x0, 0
	bl	wait
.L7:
.LBE4:
	.loc 1 55 12
	mov	w0, 0
.L10:
	.loc 1 56 1
	mov	w1, w0
	adrp	x0, :got:__stack_chk_guard;ldr	x0, [x0, :got_lo12:__stack_chk_guard]
	ldr	x3, [sp, 88]
	ldr	x2, [x0]
	subs	x3, x3, x2
	mov	x2, 0
	beq	.L11
	bl	__stack_chk_fail
.L11:
	mov	w0, w1
	ldp	x29, x30, [sp, 96]
	add	sp, sp, 112
	.cfi_restore 29
	.cfi_restore 30
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
.Letext0:
	.file 2 "/usr/lib/gcc/aarch64-linux-gnu/14/include/stddef.h"
	.file 3 "/usr/include/aarch64-linux-gnu/bits/types.h"
	.file 4 "/usr/include/unistd.h"
	.file 5 "/usr/include/aarch64-linux-gnu/sys/wait.h"
	.file 6 "/usr/include/aarch64-linux-gnu/bits/types/struct_timespec.h"
	.file 7 "/usr/include/aarch64-linux-gnu/bits/types/clockid_t.h"
	.file 8 "/usr/include/stdio.h"
	.file 9 "/usr/include/time.h"
	.section	.debug_info,"",@progbits
.Ldebug_info0:
	.4byte	0x2fe
	.2byte	0x5
	.byte	0x1
	.byte	0x8
	.4byte	.Ldebug_abbrev0
	.uleb128 0xc
	.4byte	.LASF33
	.byte	0x1d
	.4byte	.LASF0
	.4byte	.LASF1
	.8byte	.Ltext0
	.8byte	.Letext0-.Ltext0
	.4byte	.Ldebug_line0
	.uleb128 0x2
	.4byte	.LASF9
	.byte	0x2
	.byte	0xd6
	.byte	0x17
	.4byte	0x3a
	.uleb128 0x3
	.byte	0x8
	.byte	0x7
	.4byte	.LASF2
	.uleb128 0xd
	.byte	0x8
	.uleb128 0xe
	.byte	0x4
	.byte	0x5
	.string	"int"
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF3
	.uleb128 0x3
	.byte	0x2
	.byte	0x7
	.4byte	.LASF4
	.uleb128 0x3
	.byte	0x4
	.byte	0x7
	.4byte	.LASF5
	.uleb128 0x3
	.byte	0x1
	.byte	0x6
	.4byte	.LASF6
	.uleb128 0x3
	.byte	0x2
	.byte	0x5
	.4byte	.LASF7
	.uleb128 0x3
	.byte	0x8
	.byte	0x5
	.4byte	.LASF8
	.uleb128 0x2
	.4byte	.LASF10
	.byte	0x3
	.byte	0x9a
	.byte	0x19
	.4byte	0x43
	.uleb128 0x9
	.4byte	0x43
	.4byte	0x90
	.uleb128 0xa
	.4byte	0x3a
	.byte	0x1
	.byte	0
	.uleb128 0x2
	.4byte	.LASF11
	.byte	0x3
	.byte	0xa0
	.byte	0x1a
	.4byte	0x6d
	.uleb128 0x2
	.4byte	.LASF12
	.byte	0x3
	.byte	0xa9
	.byte	0x1d
	.4byte	0x43
	.uleb128 0x2
	.4byte	.LASF13
	.byte	0x3
	.byte	0xc2
	.byte	0x1b
	.4byte	0x6d
	.uleb128 0x2
	.4byte	.LASF14
	.byte	0x3
	.byte	0xc5
	.byte	0x21
	.4byte	0x6d
	.uleb128 0x3
	.byte	0x1
	.byte	0x8
	.4byte	.LASF15
	.uleb128 0xf
	.4byte	0xc0
	.uleb128 0x9
	.4byte	0xc0
	.4byte	0xdc
	.uleb128 0xa
	.4byte	0x3a
	.byte	0
	.byte	0
	.uleb128 0x2
	.4byte	.LASF16
	.byte	0x4
	.byte	0xdc
	.byte	0x13
	.4byte	0xa8
	.uleb128 0x2
	.4byte	.LASF17
	.byte	0x5
	.byte	0x1f
	.byte	0x11
	.4byte	0x74
	.uleb128 0x6
	.4byte	0xc7
	.uleb128 0x10
	.4byte	.LASF34
	.byte	0x10
	.byte	0x6
	.byte	0xb
	.byte	0x8
	.4byte	0x11f
	.uleb128 0xb
	.4byte	.LASF18
	.byte	0x10
	.byte	0xc
	.4byte	0x90
	.byte	0
	.uleb128 0xb
	.4byte	.LASF19
	.byte	0x15
	.byte	0x15
	.4byte	0xb4
	.byte	0x8
	.byte	0
	.uleb128 0x2
	.4byte	.LASF20
	.byte	0x7
	.byte	0x7
	.byte	0x15
	.4byte	0x9c
	.uleb128 0x11
	.4byte	.LASF21
	.byte	0x5
	.byte	0x53
	.byte	0x10
	.4byte	0x74
	.4byte	0x141
	.uleb128 0x1
	.4byte	0x141
	.byte	0
	.uleb128 0x6
	.4byte	0x43
	.uleb128 0x5
	.4byte	.LASF22
	.byte	0x8
	.2byte	0x16b
	.byte	0xc
	.4byte	0x43
	.4byte	0x15e
	.uleb128 0x1
	.4byte	0xf4
	.uleb128 0x12
	.byte	0
	.uleb128 0x5
	.4byte	.LASF23
	.byte	0x9
	.2byte	0x120
	.byte	0xc
	.4byte	0x43
	.4byte	0x17a
	.uleb128 0x1
	.4byte	0x11f
	.uleb128 0x1
	.4byte	0x17a
	.byte	0
	.uleb128 0x6
	.4byte	0xf9
	.uleb128 0x5
	.4byte	.LASF24
	.byte	0x4
	.2byte	0x17a
	.byte	0x10
	.4byte	0xdc
	.4byte	0x1a0
	.uleb128 0x1
	.4byte	0x43
	.uleb128 0x1
	.4byte	0x1a0
	.uleb128 0x1
	.4byte	0x2e
	.byte	0
	.uleb128 0x6
	.4byte	0x1a5
	.uleb128 0x13
	.uleb128 0x5
	.4byte	.LASF25
	.byte	0x4
	.2byte	0x173
	.byte	0x10
	.4byte	0xdc
	.4byte	0x1c7
	.uleb128 0x1
	.4byte	0x43
	.uleb128 0x1
	.4byte	0x41
	.uleb128 0x1
	.4byte	0x2e
	.byte	0
	.uleb128 0x14
	.4byte	.LASF35
	.byte	0x8
	.2byte	0x364
	.byte	0xd
	.4byte	0x1da
	.uleb128 0x1
	.4byte	0xf4
	.byte	0
	.uleb128 0x15
	.4byte	.LASF36
	.byte	0x4
	.2byte	0x30a
	.byte	0x10
	.4byte	0x74
	.uleb128 0x5
	.4byte	.LASF26
	.byte	0x4
	.2byte	0x1b5
	.byte	0xc
	.4byte	0x43
	.4byte	0x1fe
	.uleb128 0x1
	.4byte	0x141
	.byte	0
	.uleb128 0x16
	.4byte	.LASF37
	.byte	0x1
	.byte	0xc
	.byte	0x5
	.4byte	0x43
	.8byte	.LFB0
	.8byte	.LFE0-.LFB0
	.uleb128 0x1
	.byte	0x9c
	.4byte	0x2fa
	.uleb128 0x4
	.4byte	.LASF27
	.byte	0xd
	.byte	0x9
	.4byte	0x80
	.uleb128 0x2
	.byte	0x91
	.sleb128 -48
	.uleb128 0x4
	.4byte	.LASF28
	.byte	0xd
	.byte	0x13
	.4byte	0x80
	.uleb128 0x2
	.byte	0x91
	.sleb128 -40
	.uleb128 0x7
	.string	"pid"
	.byte	0x11
	.byte	0xb
	.4byte	0xe8
	.uleb128 0x3
	.byte	0x91
	.sleb128 -92
	.uleb128 0x17
	.8byte	.LBB2
	.8byte	.LBE2-.LBB2
	.4byte	0x28e
	.uleb128 0x4
	.4byte	.LASF29
	.byte	0x19
	.byte	0xe
	.4byte	0xcc
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x8
	.8byte	.LBB3
	.8byte	.LBE3-.LBB3
	.uleb128 0x7
	.string	"i"
	.byte	0x1a
	.byte	0x12
	.4byte	0x43
	.uleb128 0x3
	.byte	0x91
	.sleb128 -100
	.byte	0
	.byte	0
	.uleb128 0x8
	.8byte	.LBB4
	.8byte	.LBE4-.LBB4
	.uleb128 0x4
	.4byte	.LASF29
	.byte	0x20
	.byte	0xe
	.4byte	0xcc
	.uleb128 0x2
	.byte	0x91
	.sleb128 -32
	.uleb128 0x4
	.4byte	.LASF30
	.byte	0x21
	.byte	0x19
	.4byte	0xf9
	.uleb128 0x3
	.byte	0x91
	.sleb128 -80
	.uleb128 0x7
	.string	"end"
	.byte	0x21
	.byte	0x20
	.4byte	0xf9
	.uleb128 0x2
	.byte	0x91
	.sleb128 -64
	.uleb128 0x4
	.4byte	.LASF31
	.byte	0x2f
	.byte	0x10
	.4byte	0x2fa
	.uleb128 0x3
	.byte	0x91
	.sleb128 -88
	.uleb128 0x8
	.8byte	.LBB5
	.8byte	.LBE5-.LBB5
	.uleb128 0x7
	.string	"i"
	.byte	0x26
	.byte	0x12
	.4byte	0x43
	.uleb128 0x3
	.byte	0x91
	.sleb128 -96
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x3
	.byte	0x8
	.byte	0x4
	.4byte	.LASF32
	.byte	0
	.section	.debug_abbrev,"",@progbits
.Ldebug_abbrev0:
	.uleb128 0x1
	.uleb128 0x5
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x2
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
	.uleb128 0x3
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
	.uleb128 0x4
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
	.uleb128 0x5
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
	.uleb128 0x6
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0x21
	.sleb128 8
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x7
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
	.uleb128 0xb
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2
	.uleb128 0x18
	.byte	0
	.byte	0
	.uleb128 0x8
	.uleb128 0xb
	.byte	0x1
	.uleb128 0x11
	.uleb128 0x1
	.uleb128 0x12
	.uleb128 0x7
	.byte	0
	.byte	0
	.uleb128 0x9
	.uleb128 0x1
	.byte	0x1
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0xa
	.uleb128 0x21
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.uleb128 0x2f
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xb
	.uleb128 0xd
	.byte	0
	.uleb128 0x3
	.uleb128 0xe
	.uleb128 0x3a
	.uleb128 0x21
	.sleb128 6
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
	.uleb128 0xc
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
	.uleb128 0xd
	.uleb128 0xf
	.byte	0
	.uleb128 0xb
	.uleb128 0xb
	.byte	0
	.byte	0
	.uleb128 0xe
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
	.uleb128 0xf
	.uleb128 0x26
	.byte	0
	.uleb128 0x49
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x10
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
	.uleb128 0x11
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
	.uleb128 0x12
	.uleb128 0x18
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x13
	.uleb128 0x26
	.byte	0
	.byte	0
	.byte	0
	.uleb128 0x14
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
	.uleb128 0x3c
	.uleb128 0x19
	.uleb128 0x1
	.uleb128 0x13
	.byte	0
	.byte	0
	.uleb128 0x15
	.uleb128 0x2e
	.byte	0
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
	.uleb128 0x17
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
.LASF27:
	.string	"pipe1"
.LASF28:
	.string	"pipe2"
.LASF9:
	.string	"size_t"
.LASF30:
	.string	"start"
.LASF19:
	.string	"tv_nsec"
.LASF32:
	.string	"double"
.LASF17:
	.string	"pid_t"
.LASF16:
	.string	"ssize_t"
.LASF34:
	.string	"timespec"
.LASF3:
	.string	"unsigned char"
.LASF33:
	.string	"GNU C17 14.2.0 -mlittle-endian -mabi=lp64 -g -fasynchronous-unwind-tables -fstack-protector-strong -fstack-clash-protection"
.LASF35:
	.string	"perror"
.LASF4:
	.string	"short unsigned int"
.LASF24:
	.string	"write"
.LASF11:
	.string	"__time_t"
.LASF37:
	.string	"main"
.LASF25:
	.string	"read"
.LASF26:
	.string	"pipe"
.LASF21:
	.string	"wait"
.LASF2:
	.string	"long unsigned int"
.LASF5:
	.string	"unsigned int"
.LASF36:
	.string	"fork"
.LASF15:
	.string	"char"
.LASF31:
	.string	"elapsed"
.LASF13:
	.string	"__ssize_t"
.LASF23:
	.string	"clock_gettime"
.LASF18:
	.string	"tv_sec"
.LASF22:
	.string	"printf"
.LASF7:
	.string	"short int"
.LASF29:
	.string	"buffer"
.LASF8:
	.string	"long int"
.LASF10:
	.string	"__pid_t"
.LASF6:
	.string	"signed char"
.LASF20:
	.string	"clockid_t"
.LASF12:
	.string	"__clockid_t"
.LASF14:
	.string	"__syscall_slong_t"
	.section	.debug_line_str,"MS",@progbits,1
.LASF0:
	.string	"3-context.c"
.LASF1:
	.string	"/home/ben/SysProg-Class/demos/_bl857/2-syscall-intro"
	.ident	"GCC: (Ubuntu 14.2.0-4ubuntu2) 14.2.0"
	.section	.note.GNU-stack,"",@progbits
