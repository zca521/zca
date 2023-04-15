	.file	"sort.c"
	.option nopic
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"%d  "
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-48
	sd	ra,40(sp)
	sd	s0,32(sp)
	addi	s0,sp,48
	li	a5,5
	sw	a5,-48(s0)
	li	a5,3
	sw	a5,-44(s0)
	li	a5,4
	sw	a5,-40(s0)
	li	a5,3
	sw	a5,-28(s0)
	sw	zero,-20(s0)
	j	.L2
.L6:
	sw	zero,-24(s0)
	j	.L3
.L5:
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-32(a5)
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-32(a5)
	ble	a4,a5,.L4
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-32(a5)
	sw	a5,-32(s0)
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-32(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	sw	a4,-32(a5)
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-32(s0)
	sw	a4,-32(a5)
.L4:
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sw	a5,-24(s0)
.L3:
	lw	a5,-28(s0)
	addiw	a5,a5,-1
	sext.w	a5,a5
	lw	a4,-20(s0)
	subw	a5,a5,a4
	sext.w	a4,a5
	lw	a5,-24(s0)
	sext.w	a5,a5
	blt	a5,a4,.L5
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-28(s0)
	addiw	a5,a5,-1
	sext.w	a4,a5
	lw	a5,-20(s0)
	sext.w	a5,a5
	blt	a5,a4,.L6
	sw	zero,-20(s0)
	j	.L7
.L8:
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-32(a5)
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L7:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-28(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L8
	li	a5,0
	mv	a0,a5
	ld	ra,40(sp)
	ld	s0,32(sp)
	addi	sp,sp,48
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g5964b5cd727) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
