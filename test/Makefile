CFLAGS=-std=c11  -w 
# 指定C编译器，来构建项目
CC=riscv64-unknown-linux-gnu-gcc

SRCS=$(wildcard *.c)

OBJS=$(SRCS:.c=.s)

test:$(OBJS)

$(OBJS):%.s:%.c
	$(CC) $(CFLAGS) -S $^ -o asm/$@ 
.PHONY:clean
clean:
	-rm -rf *.s 