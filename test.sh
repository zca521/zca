#!/bin/bash
# 声明一个函数
assert() {
  # 程序运行的 期待值 为参数1
  expected="$1"
  # 输入值 为参数2
  input="$2"

  # 运行程序，传入期待值，将生成结果写入tmp.s汇编文件。
  # 如果运行不成功，则会执行exit退出。成功时会短路exit操作
#   ./all "$input" > tmp.s || exit
    ./all "$input" || exit
  # 编译rvcc产生的汇编文件
  # gcc -static -o tmp tmp.s tmp2.o
#   $RISCV/bin/riscv64-unknown-linux-gnu-gcc -static -o tmp tmp.s tmp2.o

#   # 运行生成出来目标文件
#   # ./tmp
#   $RISCV/bin/qemu-riscv64 -L $RISCV/sysroot ./tmp
  # $RISCV/bin/spike --isa=rv64gc $RISCV/riscv64-unknown-linux-gnu/bin/pk ./tmp

  # 获取程序返回值，存入 实际值
  actual="$?"

  # 判断实际值，是否为预期值
  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert  0  './test/sort.c'
assert  0  './test/jiecheng.c'