#include "riscv.h"
// 记录栈深度
static int Depth;
// 用于函数参数的寄存器们
static char *ArgReg[] = {"a0", "a1", "a2", "a3", "a4", "a5","a6","a7"};
// 当前的函数
static Obj *CurrentFunc;

// static bool left=false;
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

    assert(0);
    return 0;
}
static void genExpr(Node *Nd)
{
  if(Nd==NULL)
    return;
  switch (Nd->Kind)
  {

    case ND_ADD:
      genExpr(Nd->LHS);
      push();
      genExpr(Nd->RHS);
      pop("t0");
      printf("  add a0,t0,a0\n");
      break;
    case ND_SUB:
      genExpr(Nd->LHS);
      push();
      genExpr(Nd->RHS);
      pop("t0");
      printf("  sub a0,t0,a0\n");
      break;
    case ND_MUL:
      genExpr(Nd->LHS);
      push();
      genExpr(Nd->RHS);
      pop("t0");
      printf("  mul a0,t0,a0\n");
      break;
    case ND_DIV:
      genExpr(Nd->LHS);
      push();
      genExpr(Nd->RHS);
      pop("t0");
      printf("  div a0,t0,a0\n");
      break;
    case ND_NEG:
      genExpr(Nd->LHS);
      printf("  neg a0,a0\n");
      break;
    case ND_EQ:
    case ND_NE:
      genExpr(Nd->LHS);
      push();
      genExpr(Nd->RHS);
      pop("t0");
      printf("  xor a0,a0,t0\n");
      if(Nd->Kind==ND_EQ)
      {
        printf("  seqz  a0,a0\n");
      }
      else
      {
        printf("  snez  a0,a0\n");
      }
      break;
    case ND_LT:
    case ND_LE: 
      genExpr(Nd->LHS);
      push();
      genExpr(Nd->RHS);
      pop("t0");
      if(Nd->Kind==ND_LT)
      {
        printf("  slt a0,t0,a0\n");
      }
      else
      {
        printf("  sgt a0,t0,a0\n");
        printf("  xori  a0,a0,1\n");
      }
      break;
    case ND_ASSIGN:{
      int addr=0;
      if(Nd->LHS->Kind==ND_VAR&&Nd->LHS->Ty->Kind!=TY_ARRAY)
      {
        addr=getOffset(Nd->LHS);
        genExpr(Nd->RHS);
        printf("  sd  a0,%d(fp)\n",addr);
      }
      else if(Nd->LHS->Kind==ND_ARRAY_VALUE)
      {
        addr=getOffset(Nd->LHS->LHS);
        genExpr(Nd->LHS->RHS);
        printf("  slli  a0,a0,3\n");
        printf("  addi  a0,a0,%d\n",addr);
        printf("  add   a5,a0,fp\n");
        genExpr(Nd->RHS);
        printf("  sd  a0,0(a5)\n");
      }
      else
      {
        // printf("  lui a5,%%hi(.LC%d)\n",Nd->RHS->Val);
        // printf("  addi  a5,a5,%%lo(.LC%d)\n",Nd->RHS->Val);
        addr=getOffset(Nd->LHS);
        printf("  la  a5,.LC%d\n",Nd->RHS->Val);
        for(int i=0;i<Nd->LHS->Ty->ArrayLen;i++)
        {
          printf("  ld  a0,%d(a5)\n",i*8);
          printf("  sd  a0,%d(fp)\n",addr+i*8);
        }
      }
      break;
    }
    case ND_ADDR:
    case ND_DEREF:
      assert(0);//TODO()
      break;
    case ND_ARRAY_VALUE:{
      int addr=getOffset(Nd->LHS);
      genExpr(Nd->RHS);
      printf("  slli  a0,a0,3\n");
      printf("  addi  a0,a0,%d\n",addr);
      printf("  add a0,a0,fp\n");
      printf("  ld  a0,0(a0)\n");
      break;
    }
    case ND_ASSIGN_ARRAY:{
      break;
    }
    case ND_FUNCALL: 
    {
      int i=0;
      for(Node *arg=Nd->Args;arg;arg=arg->Next,i++)
      {
        if(i==0)
          continue;;
        genExpr(arg);
        printf("  mv  a%d,a0\n",i);
      }
      genExpr(Nd->Args);
      printf("  call  %s\n",Nd->FuncName);
      break;
    }
    case ND_NUM:
      printf("  li  a0,%d\n",Nd->Val);
      break;
    case ND_VAR:{
      int addr=getOffset(Nd);
      if(!(Nd->Ty->Kind==TY_ARRAY||Nd->Ty->Kind==TY_PTR))
        printf("  ld  a0,%d(fp)\n",addr);
      else
      {
        printf("  addi  a0,fp,%d\n",addr);
      }
      break;
    }
    case ND_STR:
      printf("  la  a0,.LC%d\n",Nd->Val);
      break;
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
    case ND_IF:
    {
      int seg=count();
      genExpr(nd->Cond);
      printf("  beq a0,x0,.L%d\n",seg);
      genStmt(nd->Then);
      seg=count();
      printf("  j   .L%d\n",seg);
      printf(".L%d:\n",seg-1);
      genStmt(nd->Els);
      printf(".L%d:\n",seg);
      break;
    }
    case ND_FOR: 
    {
      genStmt(nd->Init);
      int seg=count();
      printf("  j   .L%d\n",seg);
      seg=count();
      printf(".L%d:\n",seg);
      genStmt(nd->Then);
      genExpr(nd->Inc);
      printf(".L%d:\n",seg-1);
      genStmt(nd->Cond);
      printf("  bne a0,x0,.L%d\n",seg);

    }
   
    case ND_BLOCK:
      genStmt(nd->Body);
      break;
    case ND_EXPR_STMT:
      genExpr(nd->LHS);
      break;
    case ND_RETURN:
      genExpr(nd->LHS);
      printf("  j .L.return.%s\n", CurrentFunc->Name);

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
  printf("  .section  .rodata\n");
  printf("  .align  3\n");
  for(;nd;nd=nd->LCNext)
  {
    printf(".LC%d:\n",nd->Val);
    if(nd->Kind==ND_STR)
    {
      printf("  .string  \"%s\"\n",nd->Str);
    }
    else if(nd->Kind==ND_ASSIGN_ARRAY)
    {
      int *a=(int *)nd->ArrayVal;
      for(int i=0;i<nd->Ty->ArrayLen;i++)
      {
        printf("  .dword  %d\n",a[i]);
      }
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
    printf(".L.return.%s:\n", Fn->Name);
    printf("  mv  sp, fp\n");
    printf("  ld  fp, 0(sp)\n");
    printf("  ld  ra, 8(sp)\n");
    printf("  addi  sp, sp, 16\n");
    printf("  jr  ra\n");
    assert(Depth == 0);
  }
}

void codegen(Obj *Prog)
{
  stdout = freopen("./test/genASM/out.s", "w", stdout);
  assignLVarOffsets(Prog);
  emitText(Prog);
}