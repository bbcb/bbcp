#include <sys/types.h>
#include <sys/signal.h>
#include <setjmp.h>
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
{
	D("PtrVoid", sizeof(void *), FALSE, TRUE);
	D("int", sizeof(int), FALSE, TRUE);
	D("long", sizeof(long), FALSE, TRUE);
	D("ulong", sizeof(unsigned long), FALSE, TRUE);
	D("size_t", sizeof(size_t), FALSE, TRUE);
	D("ssize_t", sizeof(ssize_t), FALSE, TRUE);
	D("off_t", sizeof(off_t), FALSE, TRUE);
	D("clock_t", sizeof(clock_t), FALSE, TRUE);
	D("clockid_t", sizeof(clockid_t), FALSE, TRUE);
	D("time_t", sizeof(time_t), FALSE, TRUE);
	D("mode_t", sizeof(mode_t), TRUE, TRUE);
	D("pid_t", sizeof(pid_t), FALSE, TRUE);
	D("uid_t", sizeof(uid_t), FALSE, TRUE);
	D("gid_t", sizeof(gid_t), FALSE, TRUE);
	D("dev_t", sizeof(dev_t), FALSE, TRUE);
	D("ino_t", sizeof(ino_t), FALSE, TRUE);
	D("nlink_t", sizeof(nlink_t), FALSE, TRUE);
	D("blkcnt_t", sizeof(blkcnt_t), FALSE, FALSE);
	D("blksize_t", sizeof(blksize_t), FALSE, FALSE);
	D("int8_t", sizeof(int8_t), FALSE, TRUE);
	D("u_int8_t", sizeof(u_int8_t), FALSE, TRUE);
	D("int16_t", sizeof(int16_t), FALSE, TRUE);
	D("u_int16_t", sizeof(u_int16_t), FALSE, TRUE);
	D("int32_t", sizeof(int32_t), FALSE, TRUE);
	D("u_int32_t", sizeof(u_int32_t), FALSE, TRUE);
	D("int64_t", sizeof(int64_t), FALSE, TRUE);
	D("u_int64_t", sizeof(u_int64_t), FALSE, TRUE);
	D("wchar_t", sizeof(wchar_t), FALSE, TRUE);
	D("sigjmp_buf", sizeof(sigjmp_buf), FALSE, TRUE);
	D("intFlags", sizeof(int), TRUE, TRUE);
	D("FILE", sizeof(FILE), FALSE, FALSE);

	printf("%ssigset_t* = ARRAY [untagged] %d OF BYTE;\n", TABS, (int)sizeof(sigset_t));
	printf("%sPtrSigset_t* = POINTER [untagged] TO sigset_t;\n", TABS);

	return 0;
}
