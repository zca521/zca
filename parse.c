#include "riscv.h"
Obj *HEAD;
Node *LC;
static int num=0;
//program -> funDef*
//fundef -> param "(" funcParams  "{" compStmt
//param -> decType "*"* ident
//decType ->    char | int 
//funcParams -> (  param  ("," param)*  )?    ")"    

//compStmt -> (decl|stmt)* "}"

//         int     ** a[2][2]=expr     , *b=expr ;  可为 int   ;
//decl -> decType  (  def ( "," def )*  )? ";"
//def ->decArray ("=" expr|arrayVal)?
//decArray -> "*"* ident ("[" num "]")*
//arrayVal -> "{" (num ("," num)*)? |  (ch ("," ch)*)?"}"
//ch -> "'" char  "'" 

// stmt = "return" expr ";"
//        | "if" "(" expr ")" stmt ("else" stmt)?
//        | "for" "(" exprStmt exprStmt expr? ")" stmt
//        | "while" "(" expr ")" stmt
//        | "{" compoundStmt
//        | exprStmt
// exprStmt = expr? ";"
// expr = assign
// assign = equality ("=" assign)?
// equality = relational ("==" relational | "!=" relational)*
// relational = add ("<" add | "<=" add | ">" add | ">=" add)*
// add = mul ("+" mul | "-" mul)*
// mul = unary ("*" unary | "/" unary)*
// unary -> ("+" | "-")? postfix   |    ref 
// ref -> ("*" | "&") ref | postfix
// postfix -> primary ("[" expr "]")*
// primary = "(" expr ")"
//         | "sizeof" unary
//         | ident funcArgs?
//         | str
//         | num
// funcArgs -> "(" (assign ( "," assign)* )? ")"
static void funDef(Token **Rest, Token *Tok);
static Type *param(Token **Rest, Token *Tok,int Kind);
static Type *decType(Token **Rest, Token *Tok);
static Type *funcParams(Token **Rest, Token *Tok);

static Node *compStmt(Token **Rest, Token *Tok);
static Node *decl(Token **Rest, Token *Tok);
static Node *def(Token **Rest, Type *type, Token *Tok);
static Node *decArray(Token **Rest, Type *type, Token *Tok);
static Node *arrayVal(Token **Rest, Type *type, Token *Tok);

// static Node *ch(Token **Rest, Token *Tok);

static Node *stmt(Token **Rest, Token *Tok);
static Node *exprStmt(Token **Rest, Token *Tok);
static Node *expr(Token **Rest, Token *Tok);
static Node *assign(Token **Rest, Token *Tok);
static Node *equality(Token **Rest, Token *Tok);
static Node *relational(Token **Rest, Token *Tok);

static Node *add(Token **Rest, Token *Tok);
static Node *mul(Token **Rest, Token *Tok);

static Node *unary(Token **Rest, Token *Tok);
static Node *ref(Token **Rest, Token *Tok);

static Node *postfix(Token **Rest, Token *Tok);
static Node *primary(Token **Rest, Token *Tok);
static Node *funcArgs(Token **Rest,Type *type, Token *Tok);

static Obj *newFun();
bool isDecl(Token *Tok);
bool equal(Token *Tok,char*ch);
Token *skip(Token *Tok, char *Str);
Type *PointTo(Type *ty);
Node *newNode(NodeKind kind,Token *Tok);
Node *newUnary(NodeKind kind,Node *LHS,Token *Tok);
Node *newBinary(NodeKind kind,Node *LHS,Node *RHS,Token *Tok);
void addVarToHead(Node *var,bool bl);
void addVarToLCN(Node *var);
bool isExistStr(Node *ND);

bool isDecl(Token *Tok)
{
    if(equal(Tok,"int")||equal(Tok,"char"))
        return true;
    return false;
}
Node *newNode(NodeKind kind,Token *Tok)
{
    Node *node=calloc(1,sizeof(Node));
    node->Tok=Tok;
    node->Kind=kind;
    return node;
}

Node *newUnary(NodeKind kind,Node *LHS,Token *Tok)
{
    Node *node=newNode(kind,Tok);
    node->LHS=LHS;
    return node;
}

Node *newBinary(NodeKind kind,Node *LHS,Node *RHS,Token *Tok)
{
    Node *node=newNode(kind,Tok);
    node->LHS=LHS;
    node->RHS=RHS;
    return node;
}

bool equal(Token *Tok,char*ch)
{
    return strcmp(Tok->Loc,ch)==0;
}

Token *skip(Token *Tok, char *Str) {
  if (!equal(Tok,Str))
    errorTok(Tok, "expect '%s'", Str);
  return Tok->Next;
}

static Obj *newFun()
{
    Obj *obj=calloc(1,sizeof(Obj));
    obj->Next=HEAD;
    HEAD=obj;
    return obj;
}

static Type *newType(TypeKind ty)
{
    Type *type=calloc(1,sizeof(Type));
    type->Kind=ty;
    return type;
}

Type *ArrayTO(Type *ty,int length)
{
    Type *tmp=newType(TY_ARRAY);
    tmp->Base=ty;
    tmp->ArrayLen=length;
    return tmp;
}

Type *PointTo(Type *ty)
{
    Type *tmp=newType(TY_PTR);
    tmp->Base=ty;
    tmp->Size=8;
    return tmp;
}

void addVarToHead(Node *var,bool bl)
{
    if(HEAD->Locals==NULL)
    {
        HEAD->Locals=calloc(1,sizeof(Obj));
        HEAD->Locals->Name=var->Tok->Loc;
        HEAD->Locals->Ty=var->Ty;
        HEAD->Locals->IsParam=bl;
    }
    else
    {
        Obj *tmp=HEAD->Locals;
        while(tmp->Next!=NULL) tmp=tmp->Next;
        tmp->Next=calloc(1,sizeof(Obj));
        tmp->Next->Name=var->Tok->Loc;
        tmp->Next->Ty=var->Ty;
        tmp->Next->IsParam=bl;
    }

}

void addVarToLCN(Node *var)
{
    if(LC==NULL)
    {
        LC=var;
    }
    else
    {
        Node *tmp=LC;
        while(tmp->LCNext!=NULL) tmp=tmp->LCNext;
        tmp->LCNext=var;
    }
}

bool isExistStr(Node *ND)
{
    for(Node *nd=LC;nd;nd=nd->LCNext)
    {
        if(equal(nd->Tok,ND->Str))
        {
            ND->Val=nd->Val;
            return true;
        }
    }
    return false;
}
TypeKind getArrayType(Type *type)
{
    while(type->Base!=NULL) type=type->Base;
    return type->Kind;
}

Node *newIf(Node *cond,Node *then,Node *els,Token *tok)
{
    Node *ret=newNode(ND_IF,tok);
    ret->Cond=cond;
    ret->Then=then;
    ret->Els=els;
    return ret;

}

Node *newFor(Node *init,Node *cond,Node *inc,Node *then,Token *tok)
{
    Node *ret=newNode(ND_FOR,tok);
    ret->Init=init;
    ret->Cond=cond;
    ret->Inc=inc;
    ret->Then=then;
    return ret;

}

Type *findFunVar(char *name)
{
    Obj *tmp=HEAD;
    if(strcmp("printf",name)==0)
        return NULL;
    while(tmp!=NULL)
    {
        if(strcmp(tmp->Name,name)==0)
            return tmp->Params;
        tmp=tmp->Next;
    }
    errorAt(name,"not define funcname");
}

Type *findVar(char *name)
{
    Obj *tmp=HEAD->Locals;
    while(tmp!=NULL)
    {
        if(strcmp(tmp->Name,name)==0)
            return tmp->Ty;
        tmp=tmp->Next;
    }
    errorAt(name,"not define varname");
}


static Node *funcArgs(Token **Rest,Type *type, Token *Tok)
{
    Tok=skip(Tok,"(");
    if(equal(Tok,")"))
    {
        Tok=Tok->Next;
        return NULL;
    }
    Node *node=assign(&Tok,Tok);
    Node *ret=node;
    while(!equal(Tok,")"))
    {
        Tok=skip(Tok,",");
        node->Next=assign(&Tok,Tok);
        node=node->Next;
    }
    *Rest=Tok->Next;
    return ret;
}
static Node *primary(Token **Rest, Token *Tok)
{
    Node *node=NULL;
    if(equal(Tok,"("))
    {
        Tok=Tok->Next;
        node=expr(&Tok,Tok);
        Tok=skip(Tok,")");
    }
    else if(Tok->Kind==TK_IDENT&&equal(Tok->Next,"("))
    {
        node=newNode(ND_FUNCALL,Tok);
        Type *ty=findFunVar(Tok->Loc);
        node->FuncName=Tok->Loc;
        node->Ty=ty;
        node->Args=funcArgs(&Tok,ty,Tok->Next);
    }
    else if(Tok->Kind==TK_IDENT)
    {
        node=newNode(ND_VAR,Tok);
        node->Ty=findVar(Tok->Loc);
        Tok=Tok->Next;
    }
    else if(Tok->Kind==TK_STR)
    {
        node=newNode(ND_STR,Tok);
        node->Str=Tok->Loc;
        if(!isExistStr(node))
        {
            node->Val=num++;
            addVarToLCN(node);
        }
        Tok=Tok->Next;
    }
    else if(Tok->Kind==TK_NUM)
    {
        node=newNode(ND_NUM,Tok);
        node->Val=Tok->Val;
        Tok=Tok->Next;
    }
    *Rest=Tok;
    return node;
}

static Node *postfix(Token **Rest, Token *Tok)
{
    Node *node =primary(&Tok,Tok);
    Node *right;
    bool i=true;
    int j=0;
    int *array=NULL;
    while(equal(Tok,"["))
    {
        Tok=Tok->Next;
        if(i)
        {
            int mul=getArrayLen(node->Ty);
            array=calloc(mul,sizeof(int));
            Type *t=node->Ty;
            j=mul-1;
            while(t->Kind==TY_ARRAY)
            {
                array[j]=t->ArrayLen;
                j--;
                t=t->Base;
            }

            right=expr(&Tok,Tok);
            i=false;
        }
        else
        {
            right=newBinary(ND_MUL_ARRAY,right,expr(&Tok,Tok),Tok);
            right->Val=array[j+2];
            j++;
        }
        Tok=skip(Tok,"]");
    }
    if(!i)
        node=newBinary(ND_ARRAY_VALUE,node,right,Tok);
    *Rest=Tok;
    return node;
}

static Node *ref(Token **Rest, Token *Tok)
{
    Node *node=NULL;
    if(equal(Tok,"*"))
    {
        Tok=Tok->Next;
        node=newUnary(ND_DEREF,ref(&Tok,Tok),Tok);
    }
    else if(equal(Tok,"&"))
    {
        Tok=Tok->Next;
        node=newUnary(ND_ADDR,ref(&Tok,Tok),Tok);
    }
    else
        node=postfix(&Tok,Tok);
    *Rest=Tok;
    return node;
}

static Node *unary(Token **Rest, Token *Tok)
{
    Node *node=NULL;
    if(equal(Tok,"+")||equal(Tok,"-"))
    {
        if(equal(Tok,"-"))
        {
            Tok=Tok->Next;
            node=newUnary(ND_NEG,postfix(&Tok,Tok),Tok);
        }
        else
        {    
            Tok=Tok->Next;
            node=postfix(&Tok,Tok);
        }
    }
    else 
    {
        node=ref(&Tok,Tok);
    }
    *Rest=Tok;
    return node;
}

static Node *mul(Token **Rest, Token *Tok)
{
    Node *node=unary(&Tok,Tok);
    while(equal(Tok,"*")||equal(Tok,"/"))
    {
        if(equal(Tok,"*"))
        {
            Tok=Tok->Next;
            node=newBinary(ND_MUL,node,unary(&Tok,Tok),Tok);
        }
        else
        {
            Tok=Tok->Next;
            node=newBinary(ND_DIV,node,unary(&Tok,Tok),Tok);
        }
    }
    *Rest=Tok;
    return node;
}

static Node *add(Token **Rest, Token *Tok)
{
    Node *node=mul(&Tok,Tok);
    while(equal(Tok,"+")||equal(Tok,"-"))
    {
        if(equal(Tok,"+"))
        {
            Tok=Tok->Next;
            node=newBinary(ND_ADD,node,mul(&Tok,Tok),Tok);
        }
        else
        {
            Tok=Tok->Next;
            node=newBinary(ND_SUB,node,mul(&Tok,Tok),Tok);
        }
    }
    *Rest=Tok;
    return node;
}



static Node *relational(Token **Rest, Token *Tok)
{
    Node *node=add(&Tok,Tok);
    while(equal(Tok,"<")||equal(Tok,">")||equal(Tok,"<=")||equal(Tok,">="))
    {
        if(equal(Tok,"<"))
        {
            Tok=Tok->Next;
            node=newBinary(ND_LT,node,add(&Tok,Tok),Tok);
        }
        else if(equal(Tok,">"))
        {
            Tok=Tok->Next;
            node=newBinary(ND_LT,add(&Tok,Tok),node,Tok);
        }
        else if(equal(Tok,"<="))
        {
            Tok=Tok->Next;
            node=newBinary(ND_LE,node,add(&Tok,Tok),Tok);
        }
        else if(equal(Tok,">="))
        {
            Tok=Tok->Next;
            node=newBinary(ND_LE,add(&Tok,Tok),node,Tok);
        }
    }
    *Rest=Tok;
    return node;
}

static Node *equality(Token **Rest, Token *Tok)
{
    Node *node=relational(&Tok,Tok);
    while(equal(Tok,"==")||equal(Tok,"!="))
    {
        if(equal(Tok,"=="))
        {
            Tok=Tok->Next;
            node=newBinary(ND_EQ,node,relational(&Tok,Tok),Tok);
        }
        else
        {
            Tok=Tok->Next;
            node=newBinary(ND_NE,node,relational(&Tok,Tok),Tok);
        }
    }
    *Rest=Tok;
    return node;
}

static Node *assign(Token **Rest, Token *Tok)
{
    //暂未实现连续赋值
    Node *node=equality(&Tok,Tok);
    if(equal(Tok,"="))
    {
        Tok=Tok->Next;
        node=newBinary(ND_ASSIGN,node,assign(&Tok,Tok),Tok);
    }
    *Rest=Tok;
    return node;
}

static Node *expr(Token **Rest, Token *Tok)
{
    Node *node=assign(&Tok,Tok);
    *Rest=Tok;
    return node;
}

static Node *exprStmt(Token **Rest, Token *Tok)
{
    if(equal(Tok,";"))
        return NULL;
    Node *node=newNode(ND_EXPR_STMT, Tok);
    node->LHS = expr(&Tok, Tok);
    Tok=skip(Tok,";");
    *Rest=Tok;
    return node;
}

static Node *stmt(Token **Rest, Token *Tok)
{
    Node *ret=NULL;
    if(equal(Tok,"return"))
    {
        Tok=Tok->Next;
        ret=newUnary(ND_RETURN,expr(&Tok,Tok),Tok);
        Tok=skip(Tok,";");
    }
    else if(equal(Tok,"if"))
    {
        Tok=skip(Tok->Next,"(");
        Node*cond=NULL,*then=NULL,*els=NULL;
        cond=expr(&Tok,Tok);
        Tok=skip(Tok,")");
        then=stmt(&Tok,Tok);
        if(equal(Tok,"else"))
        {
            Tok=Tok->Next;
            els=stmt(&Tok,Tok);
        }
        ret=newIf(cond,then,els,Tok);
    }
    else if(equal(Tok,"for"))
    {
        Tok=skip(Tok->Next,"(");
        Node *init=NULL,*cond=NULL,*then=NULL,*inc=NULL;
        init=exprStmt(&Tok,Tok);
        cond=exprStmt(&Tok,Tok);
        if(!equal(Tok,")"))
            inc=expr(&Tok,Tok);
        Tok=skip(Tok,")");
        then=stmt(&Tok,Tok);
        ret=newFor(init,cond,inc,then,Tok);
    }
    else if(equal(Tok,"while"))
    {
        Tok=skip(Tok->Next,"(");
        Node *cond=NULL,*then=NULL;
        cond=expr(&Tok,Tok);
        Tok=skip(Tok,")");
        then=stmt(&Tok,Tok);
        ret=newFor(NULL,cond,NULL,then,Tok);
    }
    else if(equal(Tok,"{"))
    {
        Tok=Tok->Next;
        ret=compStmt(&Tok,Tok);
    }
    else
        ret=exprStmt(&Tok,Tok);
    *Rest=Tok;
    return ret;
}

static Node *arrayVal(Token **Rest, Type *type, Token *Tok)
{
    Node *ret=newNode(ND_ASSIGN_ARRAY,Tok);
    Tok=skip(Tok,"{");
    if(getArrayType(type)==TY_INT)
    {
        int *a=calloc(getArrayLen(type),sizeof(int));
        a[0]=Tok->Val;
        Tok=Tok->Next;
        int i=1;
        int len=1;
        Type *t=type;
        while(t->Kind==TY_ARRAY)
        {
            len=len*t->ArrayLen;
            t=t->Base;
        }
        while(!equal(Tok,"}")&&(i<len))
        {
            Tok=skip(Tok,",");
            a[i]=Tok->Val;
            i++;
            Tok=Tok->Next;
        }
        //数组出界暂未警告
        ret->ArrayVal=a;
    }
    ret->Ty=type;
    ret->Val=num++;
    addVarToLCN(ret);
    //字符数组暂未考虑
    *Rest=Tok->Next;
    return ret;
}

static Node *decArray(Token **Rest, Type *type, Token *Tok)
{
    while(equal(Tok,"*"))
    {
        type=PointTo(type);
        Tok=Tok->Next;
    }
    if(Tok->Kind!=TK_IDENT)
        errorTok(Tok,"expected an identifier");
    Node *ret=newNode(ND_VAR,Tok);
    Tok=Tok->Next;
    while(equal(Tok,"["))
    {
        Tok=Tok->Next;
        if(Tok->Kind!=TK_NUM)
            errorTok(Tok,"expected a number of array");
        type=ArrayTO(type,Tok->Val);
        Tok=skip(Tok->Next,"]");
    }
    ret->Ty=type;

    //未给ret->var赋值 
    addVarToHead(ret,false);
    *Rest=Tok;
    return ret;

}

static Node *def(Token **Rest, Type *type, Token *Tok)
{
    Node *ret=NULL;
    Node* node=decArray(&Tok,type,Tok);
    if(equal(Tok,"="))
    {
        Tok=Tok->Next;
        if(equal(Tok,"{"))
        {
            ret= newBinary(ND_ASSIGN,node,arrayVal(&Tok,node->Ty,Tok),Tok);
        }
        else
        {
            ret= newBinary(ND_ASSIGN,node,expr(&Tok,Tok),Tok);
        }
        ret=newUnary(ND_EXPR_STMT,ret,Tok);
    }
    *Rest=Tok;
    return ret;
}


static Node *decl(Token **Rest, Token *Tok)
{
    Node *ret=newNode(ND_BLOCK,Tok);
    Type *ty=decType(&Tok,Tok);
    if(equal(Tok,";"))
    {
        *Rest=Tok->Next;
         return NULL;
    }   
    ret->Body=calloc(1,sizeof(Node));
    Node *tmp=ret->Body;
    Node *var=def(&Tok,ty,Tok);
    if(var)
    {
        tmp->Next=var;
        tmp=tmp->Next;
    }
    while(!equal(Tok,";"))
    {
        Tok=skip(Tok,",");
        var=def(&Tok,ty,Tok);
        if(var)
        {
            tmp->Next=var;
            tmp=tmp->Next;
        }
    }
    tmp=ret->Body;
    ret->Body=tmp->Next;
    free(tmp);
    *Rest=Tok->Next;
    return ret;
    // Node* node=decArray(&Tok,Tok);

   
}



static Node *compStmt(Token **Rest, Token *Tok)
{
    Node *ret=newNode(ND_BLOCK,Tok);
    ret->Body=calloc(1,sizeof(Node));
    Node *tmp=ret->Body;
    while (!equal(Tok,"}"))
    {
        Node *isempty=NULL;
        if(isDecl(Tok))
            isempty=decl(&Tok,Tok);
        else
            isempty=stmt(&Tok,Tok);
        
        if(isempty)
        {
            tmp->Next=isempty;
            tmp=tmp->Next;
        }
    }
    tmp=ret->Body;
    ret->Body=tmp->Next;
    free(tmp);
    *Rest=Tok->Next;
    return ret;
}

static Type *funcParams(Token **Rest, Token *Tok)
{   
    if(equal(Tok,")"))
    {
        *Rest=Tok->Next;
        return NULL;
    }
        
    Type *ty=param(&Tok,Tok,1);
    Type *tmp=ty;
    while (!equal(Tok,")"))
    {
        Tok=skip(Tok,",");
        tmp->Next=param(&Tok,Tok,1);
        tmp=tmp->Next;
    }
    *Rest=Tok->Next;
    return ty;
}
static Type *decType(Token **Rest, Token *Tok)
{
    Type *ret=NULL;
    if(equal(Tok,"int"))
    {
        ret=newType(TY_INT);
        ret->Size=8;
    }
    else if(equal(Tok,"char"))
    {
        ret=newType(TY_CHAR);
        ret->Size=1;
    }
    else if(equal(Tok,"void"))
    {
        ret=newType(TY_VOID);
        ret->Size=0;
    }
    else
        errorTok(Tok, "expected an int or char");
    *Rest=Tok->Next;
    return ret;
}

static Type *param(Token **Rest, Token *Tok,int Kind)
{
    Type *ret=NULL;
    char *name=NULL;
    Type *type=decType(&Tok,Tok);
    while(equal(Tok,"*"))
    {
        type=PointTo(type);
        Tok=Tok->Next;
    }
    if(Tok->Kind==TK_IDENT)
        name=Tok->Loc;
    else
        errorTok(Tok, "expected an identifier");

    if(Kind==0)//函数返回值类型
    {
        ret=newType(TY_FUNC);
        ret->ReturnTy=type;
    }
    else if(Kind==1)//形参列表
    {
        ret=type;
    }
    
    ret->Name=name;
    if(Kind==1)
    {
        Node *node=newNode(ND_VAR,Tok);
        node->Ty=ret;
        addVarToHead(node,true);
    }
    *Rest=Tok->Next;
    return ret;
}

static void funDef(Token **Rest, Token *Tok)
{
    Obj *obj=newFun();
    LC=NULL;
    Type *type=param(&Tok,Tok,0);
    obj->Name=type->Name;
    Tok=skip(Tok,"(");
    Type *param=funcParams(&Tok,Tok);
    Tok=skip(Tok,"{");
    Node *body=compStmt(&Tok,Tok);
    obj->Ty=type;//return type
    obj->Body=body;
    obj->Params=param;
    obj->LCN=LC;
    *Rest=Tok;
}

Obj *parse(Token *Tok)
{
    HEAD=NULL;
    while(Tok->Kind!=TK_EOF)
    {
        funDef(&Tok,Tok);
    }
    return HEAD;
}