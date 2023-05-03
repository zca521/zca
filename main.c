#include "riscv.h"
int main(int Argc, char **Argv)
{
    if(Argc!=2)
    {
        error("%s: invalid number of arguments", Argv[0]);
    }
    Token *tok=tokenizeFile(Argv[1]);
    Obj *Prog = parse(tok);
    codegen(Prog);
    return 0;
}