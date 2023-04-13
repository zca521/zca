#include "riscv.h"

#define N 255  //文件每行字符最大值
// 输入的字符串
static char *CurrentInput;

static bool equal(char *ch1,char *ch2)
{
  return strcmp(ch1,ch2)==0;
}
static bool isKW(char *ch)
{
    static char *Kw[] = {"return", "if",  "else",   "for",
                       "while",  "int", "sizeof", "char" ,"void"};
    for(int i=0;i<sizeof(Kw) / sizeof(*Kw);i++)
    {
      if(equal(ch,Kw[i]))
        return true;
    }
    return false;
}
void convertKW(Token *tok)
{
  for(;tok->Kind!=TK_EOF;tok=tok->Next)
  {
      if(isKW(tok->Loc))
      {
        tok->Kind=TK_KEYWORD;
      }
  }
}
// 输出错误信息
// static文件内可以访问的函数
// Fmt为传入的字符串， ... 为可变参数，表示Fmt后面所有的参数
void error(char *Fmt, ...) {
  // 定义一个va_list变量
  va_list VA;
  // VA获取Fmt后面的所有参数
  va_start(VA, Fmt);
  // vfprintf可以输出va_list类型的参数
  vfprintf(stderr, Fmt, VA);
  // 在结尾加上一个换行符
  fprintf(stderr, "\n");
  // 清除VA
  va_end(VA);
  // 终止程序
  exit(1);
}

// 输出错误出现的位置，并退出
static void verrorAt(char *Loc, char *Fmt, va_list VA) {
  // 先输出源信息
  fprintf(stderr, "%s\n", CurrentInput);

  // 输出出错信息
  // 计算出错的位置，Loc是出错位置的指针，CurrentInput是当前输入的首地址
  int Pos = Loc - CurrentInput;
  // 将字符串补齐为Pos位，因为是空字符串，所以填充Pos个空格。
  fprintf(stderr, "%*s", Pos, "");
  fprintf(stderr, "^ ");
  vfprintf(stderr, Fmt, VA);
  fprintf(stderr, "\n");
  va_end(VA);
}

// 字符解析出错
void errorAt(char *Loc, char *Fmt, ...) {
  va_list VA;
  va_start(VA, Fmt);
  verrorAt(Loc, Fmt, VA);
  exit(1);
}


// Tok解析出错
void errorTok(Token *Tok, char *Fmt, ...) {
  va_list VA;
  va_start(VA, Fmt);
  verrorAt(Tok->Loc, Fmt, VA);
  exit(1);
}

// 生成新的Token
static Token *newToken(TokenKind Kind, char *Start, char *End) {
  // 分配1个Token的内存空间
  Token *Tok = calloc(1, sizeof(Token));
  Tok->Kind = Kind;
  Tok->Len = End - Start;
  if(Tok->Len>0)
  {
    Tok->Loc = calloc(1,Tok->Len+1);
    strncpy(Tok->Loc,Start,Tok->Len);
    Tok->Loc[Tok->Len]='\0';
  }

  return Tok;
}

bool isIdent1(char ch)
{
  if( (ch>='a'&&ch<='z') || (ch>='A'&&ch<='Z') || ch=='_')
    return true;
  else
    return false;
}

bool isIdent2(char ch)
{
  return isIdent1(ch)||(ch>='0'&&ch<='9');
}

Token *readStr(char *str)
{
  char *tmp=str+1;
  str++;
  while(!(*str=='"'||*str=='\0'))
  {
      str++;
  }
  if(!str)
    error("%s 缺少双引号",tmp);
  
  Token *tok=newToken(TK_STR,tmp,str);
  return tok;
}

static bool startsWith(char *Str, char *SubStr) {
  // 比较LHS和RHS的N个字符是否相等
  return strncmp(Str, SubStr, strlen(SubStr)) == 0;
}

int readPunct(char *str)
{
  if(startsWith(str,">=")||startsWith(str,"<=")||startsWith(str,"==")||startsWith(str,"!="))
    return 2;
  if(ispunct(*str))
    return 1;
  return 0;
}
Token *tokenize(Token *tok,char *str)
{
  while (*str) {
      // 跳过所有空白符如：空格、回车
      if (isspace(*str)) {
        ++str;
        continue;
      }

      // 解析数字
      if (isdigit(*str)) {
        // 初始化，类似于C++的构造函数
        // 我们不使用Head来存储信息，仅用来表示链表入口，这样每次都是存储在Cur->Next
        // 否则下述操作将使第一个Token的地址不在Head中。
        char *tmp=str;
        int num = strtoul(str, &str, 10);
        tok->Next = newToken(TK_NUM, tmp, str);
        // 指针前进
        tok = tok->Next;
        tok->Val=num;
        continue;
      }

      // 解析字符串字面量
      if (*str == '"') {
        tok->Next = readStr(str);
        tok = tok->Next;
        str += tok->Len+2;
        continue;
      }

      // 解析标记符或关键字
      // [a-zA-Z_][a-zA-Z0-9_]*
      if (isIdent1(*str)) {
        char *Start = str;
        do {
          ++str;
        } while (isIdent2(*str));
        tok->Next = newToken(TK_IDENT, Start, str);
        tok = tok->Next;
        continue;
      }

      // 解析操作符
      int PunctLen = readPunct(str);
      if (PunctLen) {
        tok->Next = newToken(TK_PUNCT, str, str + PunctLen);
        tok = tok->Next;
        // 指针前进Punct的长度位
        str += PunctLen;
        continue;
      }

      // 处理无法识别的字符
      errorAt(str, "invalid token");
  }

  return tok;
}


Token *tokenizeFile(char *Input)
{
    char str[N+1];
    Token tok={};

    FILE *fp=fopen(Input,"r");
    if(!fp)
    {
        error("can't open file: %s\n",Input);
    }
    Token *tmp=&tok; 
    while( fgets(str, N, fp) != NULL ) {
       tmp=tokenize(tmp,str);
    }
    fclose(fp);
    tmp->Next = newToken(TK_EOF, str, str);
    convertKW(tok.Next);
    return tok.Next;
}