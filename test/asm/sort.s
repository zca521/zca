	.file	"sort.c"
	.option nopic
	.text
	.section	.rodata
	.align	3
.LC1:
	.string	"\345\216\237\346\225\260\347\273\204\344\270\272\357\274\232"
	.align	3
.LC2:
	.string	"%d  "
	.align	3
.LC3:
	.string	"\346\216\222\345\272\217\345\220\216\344\270\272\357\274\232"
	.align	3
.LC0:
	.word	99
	.word	2
	.word	34
	.word	88
	.word	100
	.word	20
	.word	1
	.word	5
	.word	3
	.word	4
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-80
	sd	ra,72(sp)
	sd	s0,64(sp)
	addi	s0,sp,80
	lui	a5,%hi(.LC0)
	addi	a5,a5,%lo(.LC0)
	ld	a1,0(a5)
	ld	a2,8(a5)
	ld	a3,16(a5)
	ld	a4,24(a5)
	ld	a5,32(a5)
	sd	a1,-72(s0)
	sd	a2,-64(s0)
	sd	a3,-56(s0)
	sd	a4,-48(s0)
	sd	a5,-40(s0)
	li	a5,10
	sw	a5,-28(s0)
	lui	a5,%hi(.LC1)
	addi	a0,a5,%lo(.LC1)
	call	printf
	sw	zero,-20(s0)
	j	.L2
.L3:
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-56(a5)
	mv	a1,a5
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-28(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L3
	li	a0,10
	call	putchar
	sw	zero,-20(s0)
	j	.L4
.L8:
	sw	zero,-24(s0)
	j	.L5
.L7:
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-56(a5)
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-56(a5)
	ble	a4,a5,.L6
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-56(a5)
	sw	a5,-32(s0)
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-56(a5)
	lw	a5,-24(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	sw	a4,-56(a5)
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sext.w	a5,a5
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a4,-32(s0)
	sw	a4,-56(a5)
.L6:
	lw	a5,-24(s0)
	addiw	a5,a5,1
	sw	a5,-24(s0)
.L5:
	lw	a5,-28(s0)
	addiw	a5,a5,-1
	sext.w	a5,a5
	lw	a4,-20(s0)
	subw	a5,a5,a4
	sext.w	a4,a5
	lw	a5,-24(s0)
	sext.w	a5,a5
	blt	a5,a4,.L7
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L4:
	lw	a5,-28(s0)
	addiw	a5,a5,-1
	sext.w	a4,a5
	lw	a5,-20(s0)
	sext.w	a5,a5
	blt	a5,a4,.L8
	lui	a5,%hi(.LC3)
	addi	a0,a5,%lo(.LC3)
	call	printf
	sw	zero,-20(s0)
	j	.L9
.L10:
	lw	a5,-20(s0)
	slli	a5,a5,2
	addi	a5,a5,-16
	add	a5,a5,s0
	lw	a5,-56(a5)
	mv	a1,a5
	lui	a5,%hi(.LC2)
	addi	a0,a5,%lo(.LC2)
	call	printf
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L9:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-28(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	blt	a4,a5,.L10
	li	a0,10
	call	putchar
	li	a5,0
	mv	a0,a5
	ld	ra,72(sp)
	ld	s0,64(sp)
	addi	sp,sp,80
	jr	ra
	.size	main, .-main
	.ident	"GCC: (g5964b5cd727) 11.1.0"
	.section	.note.GNU-stack,"",@progbits
