#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#define CHECK_LHS 0
#define CHECK_RHS   1
#define CHECK_CALL 2

#include <stdlib.h>
#include <string.h>
#include "lexer.h"

struct symtab {
    int depth;
    int type;
    long ssize;
    char *name;
    struct symtab *next;
};



void initsymtab(void);
void addsymbol(int type);
void destroysymbols(void);
void symcheck(int check);
void arraysize(void);
void arraycheck(void);

struct symtab *get_symtab_head(void);

#endif // AST_H_INCLUDED
