#ifndef CODE_GENERATOR_H_INCLUDED
#define CODE_GENERATOR_H_INCLUDED

#define PL0C_VERSION "1.0.0"

extern int proc;
extern char __errstr;

void cg_init(void);
void cg_end(void);
void cg_const(void);
void cg_semicolon(void);
void cg_symbol(void);
void cg_var(void);
void cg_crlf(void);
void cg_procedure(void);
void cg_epilogue(void);
void cg_call(void);
void cg_odd(void);
void cg_writeint(void);
void cg_writechar(void);
void cg_readint(void);
void cg_readchar(void);
void cg_array(void);
void cg_writestr(void);
void cg_forward(void);

#endif // CODE_GENERATOR_H_INCLUDED
