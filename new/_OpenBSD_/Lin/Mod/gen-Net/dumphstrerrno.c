#include <netdb.h>
#include <stdio.h>

static void D (const char *s, int e)
{
	printf("%s, %s\n", s, hstrerror(e));
}

int main (int argc, char *argv[])
{
	D("NETDB_INTERNAL", NETDB_INTERNAL);
	D("NETDB_SUCCESS", NETDB_SUCCESS);
	D("HOST_NOT_FOUND", HOST_NOT_FOUND);
	D("TRY_AGAIN", TRY_AGAIN);
	D("NO_RECOVERY", NO_RECOVERY);
	D("NO_DATA", NO_DATA);

	return 0;
}
