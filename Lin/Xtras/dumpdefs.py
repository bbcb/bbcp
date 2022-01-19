#! /usr/bin/env python2.7
#
# A. V. Shiryaev, 2012.09, 2016.11
# simple *.h '#define' macro translator

import sys, re

INDENT = "\t"

_p1 = re.compile('^#\s*define\s+([^\s]+)\s+([A-Za-z0-9]+)\s*$')
_p2 = re.compile('^#\s*define\s+([^\s]+)\s+([^\s]+)\s+\/\*([^\*]+)\*\/\s*$')

def mkset (x):
	if x >= 0:
		s = []
		ss = 0
		i = 0
		while x != 0:
			if x % 2 != 0:
				if ss == 0:
					si = i
					sn = 1
					ss = 1
				elif i == si + sn:
					sn = sn + 1
				else:
					if sn == 1:
						s.append(str(si))
					elif sn == 2:
						s.append("%s,%s" % (si, si+1))
					else:
						s.append("%s..%s" % (si, si + sn - 1))
					si = i
					sn = 1
			i = i + 1
			x = x / 2
		if ss == 1:
			if sn == 1:
				s.append(str(si))
			elif sn == 2:
				s.append("%s,%s" % (si, si+1))
			else:
				s.append("%s..%s" % (si, si + sn - 1))
		return '{' + ','.join(s) + '}'
	else:
		return None

def fixVal (x, tip):
	if x.startswith('(') and x.endswith(')') and (x.count('(') == 1) and (x.count(')') == 1):
		x = x[1:-1]

	if x.startswith('0x'): # fix HEX
		if tip == 'i':
			if x[2] in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9'):
				return x[2:].upper() + "H"
			else:
				return "0" + x[2:].upper() + "H"
		elif tip == 's':
			return mkset(int(x, 16))
		elif tip == 'c':
			if x[2] in ('0', '1', '2', '3', '4', '5', '6', '7', '8', '9'):
				return x[2:].upper() + "X"
			else:
				return "0" + x[2:].upper() + "X"
		else:
			assert False
	elif x[0] == '0': # fix OCT
		if tip == 'i':
			return str(int(x, 8))
		elif tip == 's':
			return mkset(int(x, 8))
		elif tip == 'c':
			return fixVal("0x%x" % (int(x, 8),), tip)
		else:
			assert False
	else:
		if tip == 'i':
			return x
		else:
			try:
				y = int(x)
			except:
				return x
			else:
				if tip == 's':
					return mkset(y)
				elif tip == 'c':
					return fixVal("0x%x" % (int(y),), tip)
				else:
					assert False

def p (nIndents, sym, val, comment, val1):
	r = [ "%s%s* = %s;" % (INDENT*nIndents, sym, val) ]
	if (val1 != None) and (val != val1):
		r.append( "(* %s *)" % (val1,) )
	if comment:
		r.append( "(* %s *)" % (comment,) )
	print ' '.join(r)

def isBits (s):
	return (len(s) > 1) and s.startswith('0') # HEX or OCT

def main ():
	if len(sys.argv) == 5:
		nIndents = int(sys.argv[1])
		if sys.argv[2] == '*':
			level = None
		else:
			level = int(sys.argv[2])
		tip = sys.argv[3]
		assert tip in ('i', 's', 'c')

		fh = open(sys.argv[4], 'r')
		st = 0
		for line in fh.readlines():
			if line.startswith("#if"):
				# print line,
				st = st + 1
			elif line.startswith("#endif"):
				# print line,
				st = st - 1
			elif (level == None) or (st == level):
				r = _p2.match(line)
				if r:
					sym = r.group(1)
					val = fixVal(r.group(2), tip)
					if isBits(r.group(2)):
						val1 = fixVal(r.group(2), 's')
					else:
						val1 = None
					comment = r.group(3).strip()
					p(nIndents, sym, val, comment, val1)
				else:
					r = _p1.match(line)
					if r:
						sym = r.group(1)
						val = fixVal(r.group(2), tip)
						if isBits(r.group(2)):
							val1 = fixVal(r.group(2), 's')
						else:
							val1 = None
						comment = None
						p(nIndents, sym, val, comment, val1)
	else:
		print "usage: %s nIndents level type file.h" % (sys.argv[0],)
		print "	level:"
		print "		number: selected level"
		print "		*: all levels"
		print "	type:"
		print "		i: integer"
		print "		s: set"
		print "		c: shortchar"

if __name__ == '__main__':
	main()
