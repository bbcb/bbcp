#! /usr/bin/env python2.7

import sys, string

MOD = 'Libc'

def main ():
	while True:
		line = sys.stdin.readline()
		if line == '':
			break
		code, s = string.split(line.rstrip(), ', ', maxsplit=1)
		if s[1].lower() == s[1]:
			s = s[0].lower() + s[1:]
		print '| %s.%s: s := "%s, %s"' % (MOD, code, code, s)

if __name__ == '__main__':
	main()
