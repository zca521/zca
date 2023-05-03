	.file	"array.c"
	.option nopic
	.text
	.section	.rodata
	.align	3
.LC0:
	.word	1
	.word	2
	.word	3
	.word	4
	.word	5
	.word	6
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sd	s0,40(sp)
	addi	s0,sp,48
	lui	a5,%hi(.LC0)
	addi	a5,a5,%lo(.LC0)
	ld	a4,0(a5)
	sd	a4,-40(s0)
	ld	a4,8(a5)
	sd	a4,-32(s0)
	ld	a5,16(a5)
	sd	a5,-24(s0)
	li	a5,0
	mv	a0,a5
	ld	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g5964b5cd727) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
