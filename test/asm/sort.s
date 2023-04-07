	.file	"sort.c"
	.option nopic
	.text
	.align	1
	.globl	BubbleSort
	.type	BubbleSort, @function
BubbleSort:
	addi	sp,sp,-48
	sd	s0,40(sp)
	addi	s0,sp,48
	sd	a0,-40(s0)
	mv	a5,a1
	sw	a5,-44(s0)
	li	a5,1
	sw	a5,-20(s0)
	j	.L2
.L6:
	li	a5,1
	sw	a5,-24(s0)
	j	.L3
.L5:
	lw	a5,-24(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	lw	a3,0(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	lw	a5,0(a5)
	mv	a4,a3
	bge	a4,a5,.L4
	lw	a5,-24(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	lw	a5,0(a5)
	sw	a5,-28(s0)
	lw	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a4,a4,a5
	lw	a5,-24(s0)
	addi	a5,a5,1
	slli	a5,a5,2
	ld	a3,-40(s0)
	add	a5,a3,a5
	lw	a4,0(a4)
	sw	a4,0(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	ld	a4,-40(s0)
	add	a5,a4,a5
	lw	a4,-28(s0)
	sw	a4,0(a5)
.L4:
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sw	a5,-24(s0)
.L3:
	lw	a5,-44(s0)
	mv	a4,a5
	lw	a5,-20(s0)
	subw	a5,a4,a5
	sext.w	a4,a5
	lw	a5,-24(s0)
	sext.w	a5,a5
	ble	a5,a4,.L5
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-44(s0)
	mv	a4,a5
	lw	a5,-20(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	bgt	a4,a5,.L6
	nop
	nop
	ld	s0,40(sp)
	addi	sp,sp,48
	jr	ra
	.size	BubbleSort, .-BubbleSort
	.section	.rodata
	.align	3
.LC0:
	.word	1
	.word	2
	.word	5
	.word	3
	.word	4
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sd	ra,40(sp)
	sd	s0,32(sp)
	addi	s0,sp,48
	lui	a5,%hi(.LC0)
	addi	a5,a5,%lo(.LC0)
	ld	a4,0(a5)
	sd	a4,-40(s0)
	ld	a4,8(a5)
	sd	a4,-32(s0)
	lw	a5,16(a5)
	sw	a5,-24(s0)
	addi	a5,s0,-40
	li	a1,5
	mv	a0,a5
	call	BubbleSort
	li	a5,0
	mv	a0,a5
	ld	ra,40(sp)
	ld	s0,32(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g5964b5cd727) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
