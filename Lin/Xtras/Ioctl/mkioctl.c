#include <sys/ioctl.h>
#include <stdio.h>

#define PREFIX "\t\t"

void D (const char *s, int ctl)
{
	printf("%s%s* = %d;\n", PREFIX, s, ctl);
}

int main (int argc, char *argv[])
{
	D("FIOCLEX", FIOCLEX);
	D("FIONCLEX", FIONCLEX);
	D("FIONREAD", FIONREAD);
	D("FIONBIO", FIONBIO);
	D("FIOASYNC", FIOASYNC);

	D("TIOCMGET", TIOCMGET);
	D("TIOCMSET", TIOCMSET);
	D("TIOCEXCL", TIOCEXCL);
	D("TIOCNXCL", TIOCNXCL);
	D("TIOCOUTQ", TIOCOUTQ);
	D("TIOCSBRK", TIOCSBRK);
	D("TIOCCBRK", TIOCCBRK);
	D("TIOCMBIS", TIOCMBIS);
	D("TIOCMBIC", TIOCMBIC);

	return 0;
}
