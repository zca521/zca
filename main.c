#include "riscv.h"
void show(Token *tok)
{
    for(;tok->Kind!=TK_EOF;tok=tok->Next)
    {
        printf("%s\n",tok->Loc);
    }
}
int main(int Argc, char **Argv)
{
    if(Argc!=2)
    {
        error("%s: invalid number of arguments", Argv[0]);
    }
    Token *tok=tokenizeFile(Argv[1]);
    show(tok);
    Obj *Prog = parse(tok);
    codegen(Prog);
    return 0;
}