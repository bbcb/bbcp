#! /usr/bin/env python3

import sys

def main () -> None:
	r = []
	while True:
		line = sys.stdin.readline()
		if line == '':
			break
		s = line.rstrip()
		r.append(f'\tD("{s}", {s})')

	print("""/* this file was generated automatically */

#include <errno.h>
#include <stdio.h>
#include <string.h>

static void D (const char *s, int e)
{
	printf("%%s, %%s\\n", s, strerror(e));
}

int main (int argc, const char *argv[])
{
%s;

	return 0;
}""" % (';\n'.join(r),))

if __name__ == '__main__':
	main()
