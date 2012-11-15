#include <sys/types.h>

#include <sys/signal.h>

#include <stdlib.h>

#include <stdio.h>

#define SZ(A, B) printf("%s %d\n", A, sizeof(B));

int main ()
{
	SZ("size_t", size_t);
	SZ("ssize_t", ssize_t);
	SZ("off_t", off_t);
	SZ("clock_t", clock_t);
	SZ("mode_t", mode_t);
	SZ("pid_t", pid_t);
	SZ("uid_t", uid_t);
	SZ("sigset_t", sigset_t);

	SZ("wchar_t", wchar_t);

	return 0;
}
