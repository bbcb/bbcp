#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define TABS "\t\t"

#define FALSE (0)
#define TRUE (1)

static void D (const char *s, int sz, int set, int export)
{
	int res;

	res = printf("%s%s", TABS, s);
	if (export) {
		res = printf("*");
	}
	res = printf(" = ");
	if (sz == 1) {
		res = printf("SHORTCHAR");
	} else if (sz == 2) {
		res = printf("SHORTINT");
	} else if (sz == 4) {
		if (set) {
			res = printf("SET");
		} else {
			res = printf("INTEGER");
		}
	} else if (sz == 8) {
		if (set) {
			res = printf("ARRAY [untagged] 2 OF SET");
		} else {
			res = printf("LONGINT");
		}
	} else {
		res = printf("ARRAY [untagged] ");
		if (sz % 4 == 0) {
			if (set) {
				res = printf("%d OF SET", sz / 4);
			} else {
				res = printf("%d OF INTEGER", sz / 4);
			}
		} else {
			res = printf("%d OF SHORTCHAR", sz);
		}
	}
	res = printf(";\n");
}

int main ()
{\
	D("PtrVoid", sizeof(void *), FALSE, TRUE);
	D("short", sizeof(short), FALSE, TRUE);
	D("int", sizeof(int), FALSE, TRUE);
	D("intFlags", sizeof(int), TRUE, TRUE);
	D("size_t", sizeof(size_t), FALSE, TRUE);
	D("ssize_t", sizeof(ssize_t), FALSE, TRUE);
	D("time_t", sizeof(time_t), FALSE, TRUE);
	D("suseconds_t", sizeof(suseconds_t), FALSE, TRUE);
	D("socklen_t", sizeof(socklen_t), FALSE, TRUE);
	D("in_addr_t", sizeof(in_addr_t), FALSE, TRUE);
	D("in_port_t", sizeof(in_port_t), FALSE, TRUE);
	D("nfds_t", sizeof(nfds_t), FALSE, TRUE);
	D("sa_family_t", sizeof(sa_family_t), FALSE, TRUE);

	return 0;
}
