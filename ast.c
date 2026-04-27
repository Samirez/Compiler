#include "ast.h"
#include "token_type.h"

static struct symtab *head = NULL;

void initsymtab(void)
{
    struct symtab *newtab;

    if ((newtab = malloc(sizeof(struct symtab))) == NULL)
    {
        perror("malloc failed");
    }

    newtab->depth = 0;
    newtab->type = TOK_PROCEDURE;
    newtab->name = "main";
    newtab->next = NULL;
    head = newtab;
}

void addsymbol(int type)
{
    struct symtab *current, *newtab, *tail;
    tail = NULL;

    for (current = head; current != NULL; current = current->next)
    {
        tail = current;
        if (!strcmp(current->name, token) && current->depth == (depth-1))
        {
            if (current->type == TOK_FORWARD)
            {
                // don't print error
            } else {
                PERROR_FMT("duplicate symbol: %s", token);
            }
        }
    }

    if ((newtab = malloc(sizeof(struct symtab))) == NULL)
    {
        perror("malloc failed");
    }

    newtab->depth = depth-1;
    newtab->type = type;

    if ((newtab->name = strdup(token)) == NULL)
    {
        perror("strdup failed");
    }

    newtab->next = NULL;

    if (head == NULL)
    {
        head = newtab;
    }
    else
    {
        tail->next = newtab;
    }
}

void destroysymbols(void)
{
    struct symtab *current, *previous;

    again:
        current = head;

        while (current->next != NULL)
        {
            previous = current;
            current = current->next;
        }

        if (current->type != TOK_PROCEDURE)
        {
            free(current->name);
            free(current);
            previous->next = NULL;
            goto again;
        }
}

void symcheck(int check)
{
    struct symtab *current, *ret = NULL;
    current = head;

    while (current != NULL)
    {
        if (!strcmp(token, current->name))
        {
            ret = current;
        }
        current = current->next;
    }

    if (ret == NULL)
    {
        PERROR_FMT("undefined symbol: %s", token);
    }

    switch (check)
    {
        case CHECK_LHS:
            if (ret->type != TOK_VAR)
            {
                PERROR_FMT("must be a variable: %s", token);
            }
            break;
        case CHECK_RHS:
            if (ret->type == TOK_PROCEDURE || ret->type == TOK_FORWARD)
            {
                PERROR_FMT("must not be a procedure: %s", token);
            }
            break;
        case CHECK_CALL:
            if (ret->type != TOK_PROCEDURE && ret->type != TOK_FORWARD)
            {
                PERROR_FMT("must be a procedure: %s", token);
            }
            break;
    }
}

void arraysize(void)
{
    struct symtab *current;
    char *error_str;
    current = head;

    while (current->next != NULL)
    {
        current = current->next;
    }

    if (current->type != TOK_VAR)
    {
        perror("array must be declared with \"var\"");
    }
    errno = 0;
    current->ssize = strtol(token, &error_str, 10);

    if (errno == ERANGE || current->ssize < 1 || current->ssize > LONG_MAX || *error_str != '\0')
    {
        PERROR_FMT("invalid array size: %s", token);
        current->ssize = 0;
    }
}

void arraycheck(void)
{
    struct symtab *current, *ret = NULL;
    current = head;

    while (current != NULL)
    {
        if (!strcmp(token, current->name))
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
        PERROR_FMT("symbol %s is not an array", token);
    }
}

struct symtab *get_symtab_head(void)
{
    return head;
}
