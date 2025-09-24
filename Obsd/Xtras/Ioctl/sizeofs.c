#include <termios.h>
#include <stdio.h>

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
{
	D("int", sizeof(int), FALSE, TRUE);
	/* D("ulong", sizeof(unsigned long), FALSE, TRUE); */

	return 0;
}
