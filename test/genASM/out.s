  .section  .rodata
  .align  3
.LC0:
  .dword  1
  .dword  1
  .dword  1
  .dword  1
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  3
  .dword  3
  .dword  3
  .dword  3
.LC1:
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
  .dword  2
.LC2:
  .dword  0
  .dword  0
  .dword  0
  .dword  0
  .dword  0
  .dword  0
  .dword  0
  .dword  0
  .dword  0
.LC3:
  .string  "%3d     "
.LC4:
  .string  "\n"
  .globl main
  .text
main:
  addi sp, sp, -16
  sd ra, 8(sp)
  sd fp, 0(sp)
  mv fp, sp
  addi sp, sp, -304
  li  a0,3
  sd  a0,-8(fp)
  li  a0,4
  sd  a0,-16(fp)
  li  a0,0
  sd  a0,-24(fp)
  li  a0,0
  sd  a0,-32(fp)
  li  a0,0
  sd  a0,-40(fp)
  la  a5,.LC0
  ld  a0,0(a5)
  sd  a0,-136(fp)
  ld  a0,8(a5)
  sd  a0,-128(fp)
  ld  a0,16(a5)
  sd  a0,-120(fp)
  ld  a0,24(a5)
  sd  a0,-112(fp)
  ld  a0,32(a5)
  sd  a0,-104(fp)
  ld  a0,40(a5)
  sd  a0,-96(fp)
  ld  a0,48(a5)
  sd  a0,-88(fp)
  ld  a0,56(a5)
  sd  a0,-80(fp)
  ld  a0,64(a5)
  sd  a0,-72(fp)
  ld  a0,72(a5)
  sd  a0,-64(fp)
  ld  a0,80(a5)
  sd  a0,-56(fp)
  ld  a0,88(a5)
  sd  a0,-48(fp)
  la  a5,.LC1
  ld  a0,0(a5)
  sd  a0,-232(fp)
  ld  a0,8(a5)
  sd  a0,-224(fp)
  ld  a0,16(a5)
  sd  a0,-216(fp)
  ld  a0,24(a5)
  sd  a0,-208(fp)
  ld  a0,32(a5)
  sd  a0,-200(fp)
  ld  a0,40(a5)
  sd  a0,-192(fp)
  ld  a0,48(a5)
  sd  a0,-184(fp)
  ld  a0,56(a5)
  sd  a0,-176(fp)
  ld  a0,64(a5)
  sd  a0,-168(fp)
  ld  a0,72(a5)
  sd  a0,-160(fp)
  ld  a0,80(a5)
  sd  a0,-152(fp)
  ld  a0,88(a5)
  sd  a0,-144(fp)
  la  a5,.LC2
  ld  a0,0(a5)
  sd  a0,-304(fp)
  ld  a0,8(a5)
  sd  a0,-296(fp)
  ld  a0,16(a5)
  sd  a0,-288(fp)
  ld  a0,24(a5)
  sd  a0,-280(fp)
  ld  a0,32(a5)
  sd  a0,-272(fp)
  ld  a0,40(a5)
  sd  a0,-264(fp)
  ld  a0,48(a5)
  sd  a0,-256(fp)
  ld  a0,56(a5)
  sd  a0,-248(fp)
  ld  a0,64(a5)
  sd  a0,-240(fp)
  li  a0,0
  sd  a0,-24(fp)
  j   .L1
.L2:
  li  a0,0
  sd  a0,-32(fp)
  j   .L3
.L4:
  li  a0,0
  sd  a0,-40(fp)
  j   .L5
.L6:
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-32(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  li  t1,3
  mul t0,t0,t1
  add a0,a0,t0
  slli  a0,a0,3
  addi  a0,a0,-304
  add   a5,a0,fp
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-32(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  li  t1,3
  mul t0,t0,t1
  add a0,a0,t0
  slli  a0,a0,3
  addi  a0,a0,-304
  add a0,a0,fp
  ld  a0,0(a0)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-40(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  li  t1,4
  mul t0,t0,t1
  add a0,a0,t0
  slli  a0,a0,3
  addi  a0,a0,-136
  add a0,a0,fp
  ld  a0,0(a0)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-40(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-32(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  li  t1,3
  mul t0,t0,t1
  add a0,a0,t0
  slli  a0,a0,3
  addi  a0,a0,-232
  add a0,a0,fp
  ld  a0,0(a0)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  mul a0,t0,a0
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,0(a5)
  ld  a0,-40(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,1
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,-40(fp)
.L5:
  ld  a0,-40(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-16(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  slt a0,t0,a0
  bne a0,x0,.L6
  ld  a0,-32(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,1
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,-32(fp)
.L3:
  ld  a0,-32(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-8(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  slt a0,t0,a0
  bne a0,x0,.L4
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,1
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,-24(fp)
.L1:
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-8(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  slt a0,t0,a0
  bne a0,x0,.L2
  li  a0,0
  sd  a0,-24(fp)
  j   .L7
.L8:
  li  a0,0
  sd  a0,-32(fp)
  j   .L9
.L10:
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  ld  a0,-32(fp)
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  li  t1,3
  mul t0,t0,t1
  add a0,a0,t0
  slli  a0,a0,3
  addi  a0,a0,-304
  add a0,a0,fp
  ld  a0,0(a0)
  mv  a1,a0
  la  a0,.LC3
  call  printf
  ld  a0,-32(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,1
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,-32(fp)
.L9:
  ld  a0,-32(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,3
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  slt a0,t0,a0
  bne a0,x0,.L10
  la  a0,.LC4
  call  printf
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,1
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  add a0,t0,a0
  sd  a0,-24(fp)
.L7:
  ld  a0,-24(fp)
  # 压栈，将a0的值存入栈顶
  addi  sp, sp, -8
  sd  a0, 0(sp)
  li  a0,3
  # 弹栈，将栈顶的值存入t0
  ld  t0, 0(sp)
  addi  sp, sp, 8
  slt a0,t0,a0
  bne a0,x0,.L8
  li  a0,0
  j .L.return.main
.L.return.main:
  mv  sp, fp
  ld  fp, 0(sp)
  ld  ra, 8(sp)
  addi  sp, sp, 16
  jr  ra
