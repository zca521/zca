#include "riscv.h"
// 记录栈深度
static int Depth;
// 用于函数参数的寄存器们
static char *ArgReg[] = {"a0", "a1", "a2", "a3", "a4", "a5","a6","a7"};
// 当前的函数
static Obj *CurrentFunc;

static void genExpr(Node *Nd);
static void genStmt(Node *Nd);

// 代码段计数
static int count(void) {
  static int I = 1;
  return I++;
}

// 压栈，将结果临时压入栈中备用
// sp为栈指针，栈反向向下增长，64位下，8个字节为一个单位，所以sp-8
// 当前栈指针的地址就是sp，将a0的值压入栈
// 不使用寄存器存储的原因是因为需要存储的值的数量是变化的。
static void push(void) {
  printf("  # 压栈，将a0的值存入栈顶\n");
  printf("  addi  sp, sp, -8\n");
  printf("  sd  a0, 0(sp)\n");
  Depth++;
}

// 弹栈，将sp指向的地址的值，弹出到a1
static void pop(char *Reg) {
  printf("  # 弹栈，将栈顶的值存入%s\n", Reg);
  printf("  ld  %s, 0(sp)\n", Reg);
  printf("  addi  sp, sp, 8\n");
  Depth--;
}

// 对齐到Align的整数倍
static int alignTo(int N, int Align) {
  // (0,Align]返回Align
  return (N + Align - 1) / Align * Align;
}

static int getTySize(Type *ty)
{
  if(ty->Kind==TY_INT||ty->Kind==TY_CHAR||ty->Kind==TY_PTR)
    return ty->Size;
  if(ty->Kind==TY_ARRAY)
  {
    return ty->ArrayLen*getTySize(ty->Base);
  }
}
static void assignLVarOffsets(Obj *Prog) {
  // 为每个函数计算其变量所用的栈空间
  for (Obj *Fn = Prog; Fn; Fn = Fn->Next) {
    // 如果不是函数,则终止
    if (!Fn->IsFunction)
      continue;

    int Offset = 0;
    // 读取所有变量
    for (Obj *Var = Fn->Locals; Var; Var = Var->Next) {
      // 每个变量分配空间
      Offset += getTySize(Var->Ty);
      // 为每个变量赋一个偏移量，或者说是栈中地址
      Var->Offset = -Offset;
    }
    // 将栈对齐到16字节
    Fn->StackSize = alignTo(Offset, 16);
  }
}
int getOffset(Node *node)
{
    if(node->Kind==ND_VAR)
    {
      for(Obj *fn=CurrentFunc->Locals;fn;fn=fn->Next)
      {
        if(strcmp(node->Tok->Loc,fn->Name)==0)
          return fn->Offset;
      }
    }
}
static void genExpr(Node *Nd)
{
  switch (Nd->Kind)
  {
    case ND_ASSIGN:{
      int addr=getOffset(Nd->LHS);
      genExpr(Nd->RHS);
      printf("  sd   a0,%d(fp)\n",addr);
      break;
    }
    case ND_ADD:
      genExpr(Nd->LHS);
      printf("  mv  a1,a0\n");/////
      genExpr(Nd->RHS);
      printf("  add a0,a0,a1\n");
      break;
    case ND_NUM:
      printf("  li  a0,%d\n",Nd->Val);
      break;
    case ND_VAR:
      {
        int addr=getOffset(Nd);
        printf("  ld  a0,%d(fp)\n",addr);
        break;
      }
    default:
      break;
  }
}

static void genStmt(Node *Nd)
{
  for(Node *nd=Nd;nd;nd=nd->Next)
  {
    switch (nd->Kind)
    {
    case ND_BLOCK:
      genStmt(nd->Body);
      break;
    case ND_EXPR_STMT:
      genExpr(nd->LHS);
      break;
    case ND_RETURN:
      genExpr(nd->LHS);
      printf("  mv  sp, fp\n");
      printf("  ld  fp, 0(sp)\n");
      printf("  ld  ra, 8(sp)\n");
      printf("  addi  sp, sp, 16\n");
      printf("  jr  ra\n");
      break;
    default:
      break;
    }
  }
}

void emitData(Node *nd)
{
  if(nd==NULL)
    return ;
  printf("  .section  rodata\n");
  printf("  .align  3\n");
  for(;nd;nd=nd->LCNext)
  {
    printf(".LC%d:\n",nd->Val);
    if(nd->Kind==ND_STR)
    {
      printf("    .string  \"%s\"\n",nd->Str);
    }
  }
}
void emitText(Obj *Prog)
{
  for (Obj *Fn = Prog; Fn; Fn = Fn->Next) {
    emitData(Fn->LCN);

    if (!Fn->IsFunction)
      continue;
    printf("  .globl %s\n", Fn->Name);
    printf("  .text\n");
    printf("%s:\n", Fn->Name);
    CurrentFunc = Fn;

    printf("  addi sp, sp, -16\n");
    printf("  sd ra, 8(sp)\n");
    // 将fp压入栈中，保存fp的值
    printf("  sd fp, 0(sp)\n");
    // 将sp写入fp
    printf("  mv fp, sp\n");
    // 偏移量为实际变量所用的栈大小
    printf("  addi sp, sp, -%d\n", Fn->StackSize);

    int I = 0;
    for (Obj *Var = Fn->Locals; Var; Var = Var->Next) {
      if(!Var->IsParam)
        break;
      printf("  # 将%s寄存器的值存入%s的栈地址\n", ArgReg[I], Var->Name);
      if (Var->Ty->Size == 1)
        printf("  sb %s, %d(fp)\n", ArgReg[I++], Var->Offset);
      else if(Var->Ty->Size == 4)
        printf("  sw %s, %d(fp)\n", ArgReg[I++], Var->Offset);
      else
        printf("  sd %s, %d(fp)\n", ArgReg[I++], Var->Offset);
    }
    genStmt(Fn->Body);
    assert(Depth == 0);

    // Epilogue，后语
    // 输出return段标签


  }
}

void codegen(Obj *Prog)
{
  stdout = freopen("./test/genASM/out.s", "w", stdout);
  assignLVarOffsets(Prog);
  emitText(Prog);
}