	.arch armv8-a
	.file	"basic-write.c"
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"Hello there\n"
	.align	3
.LC1:
	.string	"write rc = %d\n"
	.text
	.align	2
	.global	main
	.type	main, %function
main:
.LFB0:
	.cfi_startproc
	stp	x29, x30, [sp, -32]!
	.cfi_def_cfa_offset 32
	.cfi_offset 29, -32
	.cfi_offset 30, -24
	mov	x29, sp
	mov	x2, 12
	adrp	x0, .LC0
	add	x1, x0, :lo12:.LC0
	mov	w0, 1
	bl	write
	str	w0, [sp, 28]
	ldr	w1, [sp, 28]
	adrp	x0, .LC1
	add	x0, x0, :lo12:.LC1
	bl	printf
	mov	w0, 0
	ldp	x29, x30, [sp], 32
	.cfi_restore 30
	.cfi_restore 29
	.cfi_def_cfa_offset 0
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0"
	.section	.note.GNU-stack,"",@progbits
