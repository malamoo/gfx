#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <errno.h>

#include "eprintf.h"

static char *name = NULL;

void eprintf(char *fmt, ...)
{
	va_list args;

	fflush(stdout);
	if (programname() != NULL)
		fprintf(stderr, "%s: ", programname());

	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);

	if (fmt[0] != '\0' && fmt[strlen(fmt)-1] == ':')
		fprintf(stderr, " %s", strerror(errno));
	fprintf(stderr, "\n");
	exit(2);
}

char *estrdup(char *s)
{
	char *t;

	t = (char *) malloc(strlen(s)+1);
	if (t == NULL)
		eprintf("estrdup(\"%.20s\") failed:", s);
	strcpy(t, s);
	return t;
}

void *emalloc(size_t n)
{
	void *p;

	p = malloc(n);
	if (p == NULL)
		eprintf("malloc of %zu bytes failed:", n);
	return p;
}

void *erealloc(void *p, size_t n)
{
	void *newp;

	newp = realloc(p, n);
	if (newp == NULL)
		eprintf("realloc of %zu bytes failed:", n);
	return newp;
}

char *programname(void)
{
	return name;
}

void setprogramname(char *str)
{
	name = estrdup(str);
}
