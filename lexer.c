#include "lexer.h"
#include "token_type.h"

char *raw;
char *token;
int depth;
int type;
size_t line = 1;

static void comment(void)
{
    int character;
    while ((character = *raw++) !='}')
    {
        if (character == '\0')
        {
            perror("unterminated comment");
        }
        if (character == '\n')
        {
            ++line;
        }
    }
}

static int ident(void)
{
    char *p;
    size_t i, len;
    p = raw;

    while (isalnum(*raw) || *raw == '_')
    {
        ++raw;
    }

    len = raw - p;
    --raw;
    free(token);

    if ((token = malloc(len + 1)) == NULL)
    {
        perror("malloc failed");
    }

    for (i = 0; i < len; i++)
    {
        token[i] = *p++;
    }

    token[i] = '\0';

    if (!strcmp(token, "const"))
    {
        return TOK_CONST;
    } else if (!strcmp(token, "var"))
    {
        return TOK_VAR;
    } else if (!strcmp(token, "procedure"))
    {
        return TOK_PROCEDURE;
    } else if (!strcmp(token, "call"))
    {
        return TOK_CALL;
    } else if (!strcmp(token, "begin"))
    {
        return TOK_BEGIN;
    } else if (!strcmp(token, "end"))
    {
        return TOK_END;
    } else if (!strcmp(token, "if"))
    {
        return TOK_IF;
    } else if (!strcmp(token, "then"))
    {
        return TOK_THEN;
    } else if (!strcmp(token, "else")){
        return TOK_ELSE;
    }
     else if (!strcmp(token, "while"))
    {
        return TOK_WHILE;
    } else if (!strcmp(token, "do"))
    {
        return TOK_DO;
    } else if (!strcmp(token, "odd"))
    {
        return TOK_ODD;
    } else if (!strcmp(token, "writeInt"))
    {
        return TOK_WRITEINT;
    } else if (!strcmp(token, "writeChar"))
    {
        return TOK_WRITECHAR;
    } else if (!strcmp(token, "readInt"))
    {
        return TOK_READINT;
    } else if (!strcmp(token, "readChar"))
    {
        return TOK_READCHAR;
    } else if (!strcmp(token, "into"))
    {
        return TOK_INTO;
    } else if (!strcmp(token, "size"))
    {
        return TOK_SIZE;
    } else if (!strcmp(token, "writeString"))
    {
        return TOK_WRITESTR;
    } else if (!strcmp(token, "forward"))
    {
        return TOK_FORWARD;
    }

    return TOK_IDENT;
}

static int number(void)
{
    char *p;
    size_t i, j = 0, len;
    p = raw;

    while (isdigit(*raw) || *raw == '_')
    {
        ++raw;
    }

    len = raw - p;
    --raw;
    free(token);

    if ((token = malloc(len + 1)) == NULL)
    {
        perror("malloc failed");
    }

    for (i = 0; i < len; i++)
    {
        if (isdigit(*p))
        {
            token[j++] = *p;
        }
        ++p;
    }

    token[j] = '\0';

    char *endptr;
    errno = 0;
    long val = strtol(token, &endptr, 10);

    if (errno == ERANGE || val < 0 || val > LONG_MAX || *endptr != '\0')
    {
        PERROR_FMT("invalid number: %s", token);
        val = 0;
    }
    return TOK_NUMBER;
}

int lex(void)
{
    again:   // skipping whitespace below
            while (*raw == ' ' || *raw == '\t' ||
                *raw == '\n' || *raw == '\r')
            {
                if (*raw++ == '\n')
                {
                    ++line;
                }
            }

            if (isalpha(*raw) || *raw == '_')
            {
                return ident();
            }
            if (isdigit(*raw)) return number();

            switch (*raw)
            {
                case '{':
                    comment();
                    goto again;
                case '.':
                case '=':
                case ',':
                case ';':
                case '#':
                case '<':
                case '>':
                case '+':
                case '-':
                case '*':
                case '/':
                case '(':
                case ')':
                case '[':
                case ']':
                    return (*raw);
                case ':':
                    if (*++raw != '=')
                        {
                            PERROR_FMT("unknown token: ':%c'", *raw);
                        }
                    return TOK_ASSIGN;
                case '\0':
                    return 0;
                default:
                    PERROR_FMT("unknown token: '%c'", *raw);
            }

            return 0;
}

int string(void)
{
    char* p;
    size_t i, len, mod = 0;
    p = ++raw;

    restart:
        while (*raw != '\'')
        {
            if (*raw == '\n' || *raw == '\0')
            {
                perror("unterminated string");
            }
            if (*raw++ == '"')
            {
                ++mod;
            }
        }
        if (*raw++ == '\'')
        {
            ++raw;
            goto restart;
        }

        --raw;
        len = raw - p + mod;

        if (len < 1)
        {
            perror("impossible string");
        }

        free(token);

        if ((token = malloc(len + 3)) == NULL)
        {
            perror("malloc failed");
        }

        token[0] = '"';

        for (i = 0; i <= len; i++)
        {
            if (*p == '\'')
            {
                token[i++] = '\\';
                ++p;
            } else if (*p == '"'){
                token[i++] = '\\';
            }

            token[i] = *p++;
        }

        token[i++] = '"';
        token[i] = '\0';

        if (len == 1 || (len == 2 && token[1] == '\\'))
        {
            token[0] = '\'';

            if (len == 1)
            {
                token[2] = '\'';
            } else {
                token[3] = '\'';
            }
            return TOK_NUMBER;
        }

    return TOK_STRING;
}
