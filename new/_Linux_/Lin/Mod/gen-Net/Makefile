# This is GNU Makefile
#    BSD       GNU
# $@    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

all: Net.txt dumphstrerrno

PY = python2.7
DEFS = defs-socket defs-so

Net.txt: Net.txt.templ ${DEFS} defs-netdb defs-in defs-sock
	${PY} ./untempl.py Net.txt.templ $@

defs-socket:
	./dumpdefs.py 3 1 i /usr/include/i386-linux-gnu/bits/socket.h | grep "	PF_" > $@
	./dumpdefs.py 3 1 i /usr/include/i386-linux-gnu/bits/socket.h | grep "	SOMAXCONN" >> $@

defs-so:
	./dumpdefs.py 3 1 s /usr/include/asm-generic/socket.h | grep "	SO_" > $@

clean:
	rm -f Net.txt ${DEFS} dumphstrerrno
