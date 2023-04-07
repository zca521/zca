#include "riscv.h"
// 记录栈深度
static int Depth;
// 用于函数参数的寄存器们
static char *ArgReg[] = {"a0", "a1", "a2", "a3", "a4", "a5","a6","a7"};
// 当前的函数
static Obj *CurrentFunc;

void codegen(Obj *Prog)
{

}