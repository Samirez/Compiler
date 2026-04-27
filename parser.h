#ifndef PARSER_H_INCLUDED
#define PARSER_H_INCLUDED

void next(void);
void expect(int match);
void parser_error(const char *msg);
void block(void);
void statement(void);
void condition(void);
void factor(void);
void term(void);
void expression(void);
void parse(void);

#endif // PARSER_H_INCLUDED
