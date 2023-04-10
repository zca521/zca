	.file	"simple.c"
	.option nopic
	.text
	.globl	c
	.section	.sdata,"aw"
	.align	2
	.type	c, @object
	.size	c, 4
c:
	.word	9
	.globl	d
	.align	2
	.type	d, @object
	.size	d, 4
d:
	.word	10
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sd	s0,24(sp)
	addi	s0,sp,32
	li	a5,3
	sw	a5,-20(s0)
	li	a5,5
	sw	a5,-24(s0)
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-24(s0)
	addw	a5,a4,a5
	sext.w	a4,a5
	lui	a5,%hi(c)
	lw	a5,%lo(c)(a5)
	addw	a5,a4,a5
	sext.w	a5,a5
	mv	a0,a5
	ld	s0,24(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g5964b5cd727) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
