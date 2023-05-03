# C编译器参数：使用C11标准，生成debug信息，禁止将未初始化的全局变量放入到common段
CFLAGS=-std=c11 -g -fno-common
# 指定C编译器，来构建项目
CC=gcc

all:main.c lex.c parse.c codegen.c riscv.h
	@$(CC) $(CFLAGS) -o $@ $^
	
test:all
	./test.sh

.PHONY:clean
clean:
	@-rm -rf *.s all