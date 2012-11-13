#! /usr/bin/env python2.7

import sys, re

def readFile (fileName):
	fh = open(fileName, 'rb')
	x = fh.read()
	fh.close()
	return x

def writeFile (fileName, x):
	fh = open(fileName, 'wb')
	fh.write(x)
	fh.close()

_p = re.compile('\%\%([^\%]+)\%\%')

def untempl (x):
	while True:
		r = _p.search(x)
		if r:
			fileName = r.group(1)
			y = readFile(fileName)
			toRepl = "%%%%%s%%%%" % (fileName,)
			x = x.replace(toRepl, y)
		else:
			break
	return x

def main ():
	if len(sys.argv) != 3:
		print "usage: %s oldfile newfile" % (sys.argv[0],)
	else:
		old = readFile(sys.argv[1])
		new = untempl(old)
		writeFile(sys.argv[2], new)

if __name__ == '__main__':
	main()
