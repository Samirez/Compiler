#include <sys/stat.h>
#include <ctype.h>
#include <fcntl.h>
#include <limits.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "lexer.h"
#include "parser.h"
#include "ast.h"

#ifndef O_BINARY
#define O_BINARY 0
#endif


static void readin(char* file);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        (void) fputs("usage: pl0c file.pl0\n", stderr);
		exit(1);
	}

	readin(argv[1]);
	initsymtab();
	parse();
	return 0;
}

static void error(const char* fmt, ...)
{
    va_list ap;
    (void) fprintf(stderr, "pl0c: error: %zu: ", line);
    va_start(ap, fmt);
    (void) vfprintf(stderr, fmt, ap);
    va_end(ap);
    (void) fputc('\n', stderr);
    exit(1);
}

static void readin(char* file)
{
    int fd;
    struct stat st;

    if (strrchr(file, '.') == NULL){
        error("file must end in '.pl0'");
    }
    if (!!strcmp(strrchr(file, '.'), ".pl0"))
		error("file must end in '.pl0'");

    if ((fd = open(file, O_RDONLY | O_BINARY)) == -1)
		error("couldn't open %s", file);

	if (fstat(fd, &st) == -1)
		error("couldn't get file size");

	if ((raw = malloc(st.st_size + 1)) == NULL)
		error("malloc failed");

	if (read(fd, raw, st.st_size) != st.st_size)
		error("couldn't read %s", file);
	raw[st.st_size] = '\0';

	(void) close(fd);
}
