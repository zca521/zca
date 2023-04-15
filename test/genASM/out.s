  .section  .rodata
  .align  3
.LC0:
  .string  "%d的阶乘是%d\n"
  .globl main
  .text
main:
  addi sp, sp, -16
  sd ra, 8(sp)
  sd fp, 0(sp)
  mv fp, sp
  addi sp, sp, -32
  li  a0,1
  sd  a0,-8(fp)
  li  a0,1
  sd  a0,-16(fp)
  li  a0,5
  sd  a0,-24(fp)
  li  a0,1
  sd  a0,-8(fp)
  j   .L1
.L2:
  ld  a0,-16(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-8(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  mul a0,t0,a0
  sd  a0,-16(fp)
  ld  a0,-8(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,1
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,-8(fp)
.L1:
  ld  a0,-8(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-24(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  sgt a0,t0,a0
  xori  a0,a0,1
  bne a0,x0,.L2
  ld  a0,-24(fp)
  mv  a1,a0
  ld  a0,-16(fp)
  mv  a2,a0
  la  a0,.LC0
  call  printf
  li  a0,0
  j .L.return.main
.L.return.main:
  mv  sp, fp
  ld  fp, 0(sp)
  ld  ra, 8(sp)
  addi  sp, sp, 16
  jr  ra
