#ifndef LEXER_H_INCLUDED
#define LEXER_H_INCLUDED

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

#define PERROR_FMT(fmt, ...) \
    do { \
        fprintf(stderr, "pl0c: error: %zu: " fmt "\n", line, __VA_ARGS__); \
        exit(1); \
    } while(0)

extern char *raw;
extern char *token;
extern int depth;
extern int type;
extern size_t line;

int lex(void);
int string(void);


#endif // LEXER_H_INCLUDED
