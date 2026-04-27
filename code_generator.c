#include "code_generator.h"
#include "lexer.h"
#include "token_type.h"
#include "ast.h"
#include <stdio.h>
#include <stdarg.h>

static void aout(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    (void) vfprintf(stdout, fmt, ap);
    va_end(ap);
}

void cg_init(void)
{
    aout("#include <stdio.h>\n");
    aout("static char __stdin[24];\n\n");
}

void cg_end(void)
{
    aout("/* PL/0 compiler %s */\n", PL0C_VERSION);
}

void cg_const(void)
{
    aout("const long %s=", token);
}

void cg_semicolon(void)
{
    aout(";\n");
}

void cg_var(void)
{
    aout("long %s;\n", token);
}

void cg_crlf(void)
{
    aout("\n");
}

void cg_procedure(void)
{
    if (proc == 0)
    {
        aout("int\n");
        aout("main(int argc, char *argv[])\n");
    } else {
        aout("void\n");
        aout("%s(void)\n", token);
    }

    aout("{\n");
}

void cg_epilogue(void)
{
    aout(";");

    if (proc == 0)
    {
        aout("return 0;");
    }

    aout("\n}\n\n");
}

void cg_call(void)
{
    aout("%s;\n", token);
}

void cg_odd(void)
{
    aout(")&1");
}

void cg_symbol(void)
{
    switch (type)
    {
        case TOK_IDENT:
        case TOK_NUMBER:
            aout("%s", token);
            break;
        case TOK_BEGIN:
            aout("{\n");
            break;
        case TOK_END:
            aout(";\n}\n");
            break;
        case TOK_IF:
            aout("if(");
            break;
        case TOK_THEN:
        case TOK_DO:
            aout(")");
            break;
        case TOK_ODD:
            aout("(");
            break;
        case TOK_WHILE:
            aout("while(");
            break;
        case TOK_EQUAL:
            aout("==");
            break;
        case TOK_COMMA:
            aout(",");
            break;
        case TOK_ASSIGN:
            aout("=");
            break;
        case TOK_HASH:
            aout("!=");
            break;
        case TOK_LESSTHAN:
            aout("<");
            break;
        case TOK_GREATERTHAN:
            aout(">");
            break;
        case TOK_PLUS:
            aout("+");
            break;
        case TOK_MINUS:
            aout("-");
            break;
        case TOK_MULTIPLY:
            aout("*");
            break;
        case TOK_DIVIDE:
            aout("/");
            break;
        case TOK_LPAREN:
            aout("(");
            break;
        case TOK_RPAREN:
            aout(")");
    }
}

void cg_writeint(void)
{
    aout("(void) fprintf(stdout, \"%%ld\", (long) %s);", token);
}

void cg_writechar(void)
{
    aout("(void) fprintf(stdout, \"%%c\", (unsigned char) %s;", token);
}

void cg_readint(void)
{
    aout("(void) fgets(__stdin, sizeof(__stdin), stdin);\n");
	aout("if(__stdin[strlen(__stdin) - 1] == '\\n')");
	aout("__stdin[strlen(__stdin) - 1] = '\\0';");
	aout("%s=(long) strtonum(__stdin, LONG_MIN, LONG_MAX, &__errstr);\n",
	    token);
	aout("if(__errstr!=NULL){");
	aout("(void) fprintf(stderr, \"invalid number: %%s\\n\", __stdin);");
	aout("exit(1);");
	aout("}");
}

void cg_readchar(void)
{
    aout("%s=(unsigned char) fgetc(stding);", token);
}

void cg_array(void)
{
    aout("[%s]", token);
}

void cg_writestr(void)
{
    struct symtab *current, *ret;

    if (type == TOK_IDENT)
    {
        current = get_symtab_head();
        while (current != NULL)
        {
            if (!strcmp(current->name, token))
            {
                ret = current;
            }
            current = current->next;
        }
        if (ret == NULL)
        {
            PERROR_FMT("undefined symbol: %s", token);
        }
        if (ret->ssize == 0)
        {
            perror("writeStr requires an array");
        }
        aout("__writestridx = 0;\n");
        aout("while(%s[__writestridx]!='\\0'&&__writestridx<%ld)\n", token, ret->ssize);
        aout("(void)fputc((unsigned char)%s[__writestridx++], stdout);\n", token);

    } else {
        aout("(void)fprintf(stdout, %s);\n", token);
    }
}

void cg_forward(void)
{
    aout("static void %s(void);", token);
}
