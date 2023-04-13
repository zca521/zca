  .globl main
  .text
main:
  addi sp, sp, -16
  sd ra, 8(sp)
  sd fp, 0(sp)
  mv fp, sp
  addi sp, sp, -16
  li  a0,3
  sd   a0,-8(fp)
  li  a0,1
  sd   a0,-16(fp)
  ld  a0,-8(fp)
  mv  a1,a0
  li  a0,1
  add a0,a0,a1
  mv  a1,a0
  ld  a0,-16(fp)
  add a0,a0,a1
  sd   a0,-8(fp)
  ld  a0,-8(fp)
  mv  sp, fp
  ld  fp, 0(sp)
  ld  ra, 8(sp)
  addi  sp, sp, 16
  jr  ra
