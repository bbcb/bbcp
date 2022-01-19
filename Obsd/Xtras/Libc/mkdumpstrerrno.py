#! /usr/bin/env python2.7

import sys

def main ():
	r = []
	while True:
		line = sys.stdin.readline()
		if line == '':
			break
		s = line.split()[0][:-1]
		r.append("\tD(\"%s\", %s)" % (s, s))

	print """/* this file was generated automatically */

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
}""" % (';\n'.join(r),)

if __name__ == '__main__':
	main()
