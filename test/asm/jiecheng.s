	.file	"jiecheng.c"
	.option nopic
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"%d\347\232\204\351\230\266\344\271\230\346\230\257%d\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-32
	sd	ra,24(sp)
	sd	s0,16(sp)
	addi	s0,sp,32
	li	a5,1
	sw	a5,-20(s0)
	li	a5,1
	sw	a5,-24(s0)
	li	a5,5
	sw	a5,-28(s0)
	li	a5,1
	sw	a5,-20(s0)
	j	.L2
.L3:
	lw	a5,-24(s0)
	mv	a4,a5
	lw	a5,-20(s0)
	mulw	a5,a4,a5
	sw	a5,-24(s0)
	lw	a5,-20(s0)
	addiw	a5,a5,1
	sw	a5,-20(s0)
.L2:
	lw	a5,-20(s0)
	mv	a4,a5
	lw	a5,-28(s0)
	sext.w	a4,a4
	sext.w	a5,a5
	ble	a4,a5,.L3
	lw	a4,-24(s0)
	lw	a5,-28(s0)
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
