# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

all: Termios.txt

PY = python2.7
DEFS = defs-termios

Termios.txt: Termios.txt.templ ${DEFS}
	${PY} ./untempl.py Termios.txt.templ $@

defs-termios:
	./dumpdefs.py 2 0 i ${INCDIR}/i386-linux-gnu/bits/termios.h > $@

clean:
	rm -f Termios.txt ${DEFS}
