CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g
SRCS = main.c lexer.c parser.c code_generator.c ast.c
PROG = pl0c
OBJS = $(SRCS:.c=.o)
MSYS_BIN = C:/msys64/ucrt64/bin
MSYS_SH = C:/msys64/usr/bin/sh.exe

export PATH := $(MSYS_BIN);$(PATH)

all: $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS)

test:
	cd tests && $(MSYS_SH) ./test.sh

clean: 
	rm -f ${PROG} $(OBJS) ${PROG}.exe

fclean: clean
	rm -f $(PROG)

.PHONY: all clean fclean   