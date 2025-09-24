#! /usr/bin/env python3

import sys

MOD = 'Libc'

def main () -> None:
	while True:
		line = sys.stdin.readline()
		if line == '':
			break
		code, s = line.rstrip().split(', ', maxsplit=1)
		if s[1].lower() == s[1]:
			s = s[0].lower() + s[1:]
		print(f'| {MOD}.{code}: s := "{code}, {s}"')

if __name__ == '__main__':
	main()
