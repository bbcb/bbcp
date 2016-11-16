#include <locale.h>
#include <stdio.h>

#define PREFIX "\t\t"

void D (const char *s, int ctl)
{
	printf("%s%s* = %d;\n", PREFIX, s, ctl);
}

int main (int argc, char *argv[])
{
	D("LC_CTYPE", LC_CTYPE);
	D("LC_NUMERIC", LC_NUMERIC);
	D("LC_TIME", LC_TIME);
	D("LC_COLLATE", LC_COLLATE);
	D("LC_MONETARY", LC_MONETARY);
	D("LC_MESSAGES", LC_MESSAGES);
	D("LC_ALL", LC_ALL);
	D("LC_PAPER", LC_PAPER);
	D("LC_NAME", LC_NAME);
	D("LC_ADDRESS", LC_ADDRESS);
	D("LC_TELEPHONE", LC_TELEPHONE);
	D("LC_MEASUREMENT", LC_MEASUREMENT);
	D("LC_IDENTIFICATION", LC_IDENTIFICATION);

	return 0;
}
