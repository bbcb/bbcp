#! /usr/bin/env python2.7

import sys, string

MOD = 'Libc'

def main ():
	while True:
		line = sys.stdin.readline()
		if line == '':
			break
		code, s = string.split(line.rstrip(), ', ', maxsplit=1)
		s1 = s[0].lower() + s[1:]
		print '| %s.%s: s := "%s, %s"' % (MOD, code, code, s1)

if __name__ == '__main__':
	main()
