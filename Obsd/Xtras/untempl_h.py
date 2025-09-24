#! /usr/bin/env python3

from dataclasses import dataclass
import sys, os, re, argparse, subprocess, tempfile

# set representation of integer
def mkset (x: int) -> str:
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
			x = x // 2
		if ss == 1:
			if sn == 1:
				s.append(str(si))
			elif sn == 2:
				s.append("%s,%s" % (si, si+1))
			else:
				s.append("%s..%s" % (si, si + sn - 1))
		return '{' + ','.join(s) + '}'
	else:
		print(x)
		raise RuntimeError()
		return str(x)

def fix_val (x: str, type_: str) -> str | None:
	if x.startswith('0x'): # fix HEX
		if type_ in ('i', 'ul'):
			if x[2] in '0123456789':
				return x[2:].upper() + "H"
			else:
				return "0" + x[2:].upper() + "H"
		elif type_ == 's':
			return mkset(int(x, 16))
		elif type_ == 'c':
			if x[2] in '0123456789':
				return x[2:].upper() + "X"
			else:
				return "0" + x[2:].upper() + "X"
		else:
			raise RuntimeError()
	elif x[0] == '0': # fix OCT
		if x[-1] == 'U':
			x = x[:-1]
		if type_ in ('i', 'ul'):
			return str(int(x, 8))
		elif type_ == 's':
			return mkset(int(x, 8))
		elif type_ == 'c':
			return fix_val(f"0x{int(x, 8):x}", type_)
		else:
			raise RuntimeError()
	else:
		try:
			y = int(x)
		except:
			return None
		else:
			if type_ in ('i', 'ul'):
				return x
			elif type_ == 's':
				return mkset(y)
			elif type_ == 'c':
				return fix_val(f"0x{int(y):x}", type_)
			else:
				raise RuntimeError()

def p (indent: str, sym: str, val: str, comment: str | None = None) -> str:
	r = [ f"{indent}{sym}* = {val};" ]
	if comment:
		r.append( f"(* {comment} *)" )
	return ' '.join(r)

def readFile (path: str | bytes) -> bytes:
	with open(path, 'rb') as fh:
		return fh.read()

def writeFile (path: str | bytes, x: bytes) -> None:
	with open(path, 'wb') as fh:
		fh.write(x)

_p0 = re.compile(rb'%%([^%]+)%%')
_p1 = re.compile(rb'@@([^@]+)@@')

def untempl_files (x: bytes) -> bytes:
	while True:
		r = _p0.search(x)
		if r:
			path = r.group(1)
			y = readFile(path)
			toRepl = b"%%" + path + b"%%"
			x = x.replace(toRepl, y)
		else:
			break
	return x

def untempl_specs (x: bytes, cc: str, m: int, defs: list[str], done: set[str]) -> bytes:
	while True:
		r = _p1.search(x)
		if r:
			spec = r.group(1)
			y = process_spec(spec.decode('utf-8'), cc, m, defs, done).encode('utf-8')
			toRepl = b"@@" + spec + b"@@"
			x = x.replace(toRepl, y)
		else:
			break
	return x

@dataclass
class Ctx:
	cc: str
	m: int
	incs: list[str]
	defs: list[str]
	type_: str
	verbose: bool
	res: list[str]
	done: set[str]
	d: dict[str,str] | None

def process_one_preprocessor (ctx: Ctx, key: str) -> bool:
	assert key not in ctx.done
	a = [ctx.cc, f"-m{ctx.m}", "-E"]
	for define in ctx.defs:
		a.append(f"-D{define}")
	for inc in ctx.incs:
		a.extend(["-include", inc])
	a.append("-")
	o = subprocess.check_output(a, input=key+"\n", text=True, encoding="utf-8").splitlines()[-1]
	if ctx.verbose:
		print(key, repr(o))
	if o != key:
		val = fix_val(o, ctx.type_)
		if val is not None:
			ctx.res.append(p("\t\t", key, val, comment=None))
			ctx.done.add(key)
			ok = True
		else:
			ok = False
	else:
		ok = False
	return ok

def process_one_c (ctx: Ctx, key: str) -> bool:
	assert key not in ctx.done
	temp_file = tempfile.NamedTemporaryFile(mode="w+", suffix=".c", delete=False)
	try:
		#for inc in ctx.incs:
		#	temp_file.write(f"#include <{inc}>\n")
		temp_file.write("#include <stdio.h>\n\n")
		temp_file.write("int main (int argc, const char *argv[]) {\n")
		if ctx.type_ == 'ul':
			temp_file.write(f'	printf("%lu\\n", {key});\n')
		else:
			temp_file.write(f'	printf("%d\\n", {key});\n')
		temp_file.write(f'	return 0;\n')
		temp_file.write("}\n")
		temp_file.close()

		a = [ctx.cc, f"-m{ctx.m}"]
		for define in ctx.defs:
			a.append(f"-D{define}")
		for inc in ctx.incs:
			a.extend(["-include", inc])
		a.append("-o")
		exe = temp_file.name[:-2]
		a.append(exe)
		a.append(temp_file.name)

		# o = subprocess.check_output(["cat", temp_file.name], stdin=subprocess.DEVNULL, text=True, encoding="utf-8")
		# print(o)

		o = subprocess.check_output(a, stdin=subprocess.DEVNULL, text=True, encoding="utf-8")
	finally:
		if os.path.exists(temp_file.name):
			os.remove(temp_file.name)

	assert os.path.exists(exe)

	try:
		o = subprocess.check_output([f"{os.path.realpath(exe)}"], stdin=subprocess.DEVNULL, text=True, encoding="utf-8").splitlines()[-1]
	finally:
		if os.path.exists(exe):
			os.remove(exe)

	if ctx.verbose:
		print(f"result = {o}")

	val = fix_val(o, ctx.type_)
	assert val is not None
	ctx.res.append(p("\t\t", key, val, comment=None))
	ctx.done.add(key)
	ok = True
	return ok

def process_one (ctx: Ctx, key: str) -> bool:
	ok = process_one_preprocessor(ctx, key)
	if not ok:
		ok = process_one_c(ctx, key)
	return ok

def process_prefix_entry_rec (ctx: Ctx, key: str, v: str, level: int = 0) -> None:
	if level == 10:
		print("key:", key)
		print("v:", v)
		raise RuntimeError()
	val = fix_val(v, ctx.type_)
	if val is None:
		assert ctx.d is not None
		if v in ctx.d:
			if v not in ctx.done:
				process_prefix_entry_rec(ctx, v, ctx.d[v], level = level + 1)
			val = v
	if val is not None:
		ctx.res.append(p("\t\t", key, val, comment=None))
		ctx.done.add(key)
	else:
		ok = process_one(ctx, key)
		assert ok

def process_prefix (ctx: Ctx, prefix: str) -> None:
	a = [ctx.cc, f"-m{ctx.m}", "-dM", "-E"]
	for define in ctx.defs:
		a.append(f"-D{define}")
	for inc in ctx.incs:
		a.extend(["-include", inc])
	a.append("-")
	if ctx.d is None:
		ctx.d = {}
		for line in subprocess.check_output(a, stdin=subprocess.DEVNULL, text=True, encoding="utf-8").splitlines():
			if line.startswith(f'#define '):
				x = line.rstrip().split(maxsplit=2)
				if len(x) == 3:
					if x[1].startswith("__") or x[1].startswith("pseudo_"):
						pass
					else:
						ctx.d[x[1]] = x[2]
	for key, v in ctx.d.items():
		if key.startswith(prefix) and (key not in ctx.done):
			if key != v:
				process_prefix_entry_rec(ctx, key, v, level=0)
			else:
				ok = process_one(ctx, key)
				assert ok

# i=include (-include)
# d=define (-D)
# t=type
def process_spec (spec: str, cc: str, m: int, global_defs: list[str], done: set[str], verbose: bool = False) -> str:
	incs: list[str] = []
	defs: list[str] = []
	type_: str = "i"
	keys: list[str] = []
	for s in spec.split(':'):
		if '=' in s:
			k, v = s.split('=')
			match k:
				case "i":
					incs.append(v)
				case "d":
					defs.append(v)
				case "t":
					assert v in ('i', 'ul', 's', 'c')
					type_ = v
				case _:
					raise RuntimeError()
		else:
			for v in s.split(','):
				keys.append(v)

	res: list[str] = []

	ctx = Ctx(
		cc=cc,
		m=m,
		incs=incs,
		defs=global_defs+defs,
		type_=type_,
		verbose=verbose,
		done=done,
		res=res,
		d=None
	)

	for i, key in enumerate(keys):
		if key.endswith('*'):
			process_prefix(ctx, key[:-1])
		else:
			ok = process_one(ctx, key)
			assert ok

	return '\n'.join(res)

def main () -> None:
	ap = argparse.ArgumentParser()
	ap.add_argument("-cc", type=str, default="cc")
	ap.add_argument("-m", type=int, default=32)
	ap.add_argument("-d", action="append", default=[])
	ap.add_argument("templatefile")
	ap.add_argument("outfile")
	args = ap.parse_args()

	done: set[str] = set()
	writeFile(args.outfile, untempl_specs(untempl_files(readFile(args.templatefile)), cc=args.cc, m=args.m, defs=args.d, done=done))

if __name__ == '__main__':
	main()
