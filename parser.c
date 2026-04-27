#include "parser.h"
#include "code_generator.h"
#include "lexer.h"
#include "token_type.h"
#include "ast.h"
#include <stdio.h>
#include <stdlib.h>

int proc;

void parse(void)
{
    next();
    block();
    while (type == TOK_SEMICOLON)
    {
        expect(TOK_SEMICOLON);
        statement();
    }
    expect(TOK_DOT);

    if (type != 0)
    {
        parser_error("extra tokens at end of file");
    }
    cg_end();
}

void parser_error(const char *msg)
{
    (void) fprintf(stderr, "pl0c: error: %zu: %s\n", line, msg);
    exit(1);
}

void next(void)
{
    type = lex();
    ++raw;
}

void expect(int match)
{
    if (match != type)
    {
        parser_error("syntax error");
    }
    next();
}

void block(void)
{
    if (depth++ > 1)
    {
        parser_error("nesting depth exceeded");
    }
    while (type == TOK_CONST)
    {
        expect(TOK_CONST);
        addsymbol(TOK_CONST);
        cg_const();
        expect(TOK_IDENT);
        expect(TOK_EQUAL);
        cg_symbol();
        expect(TOK_NUMBER);
        while (type == TOK_COMMA)
        {
            expect(TOK_COMMA);
            addsymbol(TOK_CONST);
            cg_const();
            expect(TOK_IDENT);
            expect(TOK_EQUAL);
            cg_symbol();
            expect(TOK_NUMBER);
        }
        expect(TOK_SEMICOLON);
    }
    while (type == TOK_VAR)
    {
        expect(TOK_VAR);
        addsymbol(TOK_VAR);
        cg_var();
        expect(TOK_IDENT);
        if (type == TOK_SIZE)
        {
            expect(TOK_SIZE);
            arraysize();
            cg_array();
            expect(TOK_NUMBER);
        }
        while (type == TOK_COMMA)
        {
            expect(TOK_COMMA);
            addsymbol(TOK_VAR);
            cg_var();
            expect(TOK_IDENT);
            if (type == TOK_SIZE)
            {
                expect(TOK_SIZE);
                arraysize();
                cg_array();
                expect(TOK_NUMBER);
            }
        }
        expect(TOK_SEMICOLON);
        cg_crlf();
    }

    while (type == TOK_FORWARD)
    {
        expect(TOK_FORWARD);
        if (type == TOK_IDENT)
        {
            addsymbol(TOK_FORWARD);
            cg_forward();
        }
        expect(TOK_IDENT);
        expect(TOK_SEMICOLON);
    }

    while (type == TOK_PROCEDURE)
    {
        proc = 1;
        expect(TOK_PROCEDURE);
        if (type == TOK_IDENT)
        {
            addsymbol(TOK_PROCEDURE);
            cg_procedure();
        }
        expect(TOK_IDENT);
        expect(TOK_SEMICOLON);
        block();
        expect(TOK_SEMICOLON);
        proc = 0;
        destroysymbols();
    }

    while (type == TOK_VAR)
    {
        expect(TOK_VAR);
        addsymbol(TOK_VAR);
        cg_var();
        expect(TOK_IDENT);
        if (type == TOK_SIZE)
        {
            expect(TOK_SIZE);
            arraysize();
            cg_array();
            expect(TOK_NUMBER);
        }
        while (type == TOK_COMMA)
        {
            expect(TOK_COMMA);
            addsymbol(TOK_VAR);
            cg_var();
            expect(TOK_IDENT);
            if (type == TOK_SIZE)
            {
                expect(TOK_SIZE);
                arraysize();
                cg_array();
                expect(TOK_NUMBER);
            }
        }
        expect(TOK_SEMICOLON);
        cg_crlf();
    }

    if (proc == 0)
    {
        cg_procedure();
    }

    statement();

    cg_epilogue();

    if (--depth < 0)
    {
        parser_error("nesting depth fell below 0");
    }
}

void statement(void)
{
    switch (type)
    {
        case TOK_IDENT:
            symcheck(CHECK_LHS);
            cg_symbol();
            expect(TOK_IDENT);
            if (type == TOK_LBRACK)
            {
                arraycheck();
                cg_symbol();
                expect(TOK_LBRACK);
                expression();
                if (type == TOK_RBRACK)
                {
                    cg_symbol();
                }
                expect(TOK_RBRACK);
            }
            if (type == TOK_ASSIGN)
            {
                cg_symbol();
            }
            expect(TOK_ASSIGN);
            expression();
            break;
        case TOK_CALL:
            expect(TOK_CALL);
            if (type == TOK_IDENT)
            {
                symcheck(CHECK_CALL);
                cg_call();
            }
            expect(TOK_IDENT);
            break;
        case TOK_BEGIN:
            cg_symbol();
            expect(TOK_BEGIN);
            statement();
            while (type == TOK_SEMICOLON)
            {
                cg_semicolon();
                expect(TOK_SEMICOLON);
                statement();
            }
            if (type == TOK_END)
            {
                cg_symbol();
            }
            expect(TOK_END);
            break;
        case TOK_IF:
            cg_symbol();
            expect(TOK_IF);
            condition();
            if (type == TOK_THEN)
            {
                cg_symbol();
            }
            expect(TOK_THEN);
            statement();
            break;
        case TOK_WHILE:
            cg_symbol();
            expect(TOK_WHILE);
            condition();
            if (type == TOK_DO)
            {
                cg_symbol();
            }
            expect(TOK_DO);
            statement();
            break;
        case TOK_WRITEINT:
            expect(TOK_WRITEINT);
            if (type == TOK_IDENT || type == TOK_NUMBER)
            {
                if (type == TOK_IDENT)
                {
                    symcheck(CHECK_RHS);
                    cg_writeint();
                }
            }
            if (type == TOK_IDENT)
            {
                expect(TOK_IDENT);
            } else if (type == TOK_NUMBER)
            {
                expect(TOK_NUMBER);
            } else {
                parser_error("writeInt takes an identifier or a number");
            }
            break;
        case TOK_WRITECHAR:
            expect(TOK_WRITECHAR);
            if (type == TOK_IDENT || type == TOK_NUMBER)
            {
                if (type == TOK_IDENT)
                {
                    symcheck(CHECK_RHS);
                    cg_writechar();
                }
            }
            if (type == TOK_IDENT)
            {
                expect(TOK_IDENT);
            } else if (type == TOK_NUMBER)
            {
                expect(TOK_NUMBER);
            } else {
                parser_error("writeChar takes an identifier or a number");
            }
            break;
        case TOK_READINT:
            expect(TOK_READINT);
            if (type == TOK_INTO)
            {
                expect(TOK_INTO);
            }
            if (type == TOK_IDENT)
            {
                symcheck(CHECK_LHS);
                cg_readint();
            }
            expect(TOK_IDENT);
            break;
        case TOK_READCHAR:
            expect(TOK_READCHAR);
            if (type == TOK_INTO)
            {
                expect(TOK_INTO);
            }
            if (type == TOK_IDENT)
            {
                symcheck(CHECK_LHS);
                cg_readchar();
            }
            expect(TOK_IDENT);
            break;
        case TOK_WRITESTR:
            expect(TOK_WRITESTR);
            if (type == TOK_IDENT || type == TOK_STRING)
            {
                if (type == TOK_IDENT)
                {
                    symcheck(CHECK_LHS);
                }
                cg_writestr();
                if (type == TOK_IDENT)
                {
                    expect(TOK_IDENT);
                } else {
                    expect(TOK_STRING);
                }
            } else {
                parser_error("writeStr takes an array or a string");
            }
            break;
    }
}

void condition(void)
{
    if (type == TOK_ODD)
    {
        cg_symbol();
        expect(TOK_ODD);
        expression();
        cg_odd();
    } else {
        expression();
        switch (type)
        {
            case TOK_EQUAL:
            case TOK_HASH:
            case TOK_LESSTHAN:
            case TOK_GREATERTHAN:
                cg_symbol();
                next();
                break;
            default:
                parser_error("invalid conditional");
        }
        expression();
    }
}

void factor(void)
{
    switch (type)
    {
        case TOK_IDENT:
            symcheck(CHECK_RHS);
            cg_symbol();
            expect(TOK_IDENT);
            if (type == TOK_LBRACK)
            {
                arraycheck();
                cg_symbol();
                expect(TOK_LBRACK);
                expression();
                if (type == TOK_RBRACK)
                {
                    cg_symbol();
                }
                expect(TOK_RBRACK);
            }
            break;
        case TOK_NUMBER:
            cg_symbol();
            next();
            break;
        case TOK_LPAREN:
            cg_symbol();
            expect(TOK_LPAREN);
            expression();
            if (type == TOK_RPAREN)
            {
                cg_symbol();
            }
            expect(TOK_RPAREN);
    }
}

void term(void)
{
    factor();

    while (type == TOK_MULTIPLY || type == TOK_DIVIDE)
    {
        cg_symbol();
        next();
        factor();
    }
}

void expression(void)
{
    if (type == TOK_PLUS || type == TOK_MINUS)
    {
        cg_symbol();
        next();
    }

    term();

    while (type == TOK_PLUS || type == TOK_MINUS)
    {
        cg_symbol();
        next();
        term();
    }
}
