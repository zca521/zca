	.file	"simple.c"
	.option nopic
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"%d  %d\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sd	ra,24(sp)
	sd	s0,16(sp)
	addi	s0,sp,32
	li	a5,999
	sw	a5,-32(s0)
	li	a5,999
	sw	a5,-28(s0)
	li	a5,999
	sw	a5,-24(s0)
	li	a5,888
	sw	a5,-28(s0)
	lw	a5,-28(s0)
	sw	a5,-20(s0)
	lw	a4,-28(s0)
	lw	a5,-20(s0)
	mv	a2,a4
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	li	a5,0
	mv	a0,a5
	ld	ra,24(sp)
	ld	s0,16(sp)
	addi	sp,sp,32
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g5964b5cd727) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
