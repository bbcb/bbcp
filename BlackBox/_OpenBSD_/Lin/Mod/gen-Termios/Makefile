# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

all: Termios.txt

PY = python2.7
DEFS = defs-termios

Termios.txt: Termios.txt.templ ${DEFS}
	${PY} ./untempl.py Termios.txt.templ ${.TARGET}

defs-termios:
	./dumpdefs.py 2 0 i ${INCDIR}/termios.h > ${.TARGET}
	./dumpdefs.py 2 1 i ${INCDIR}/termios.h | grep -v compat >> ${.TARGET}
	./dumpdefs.py 2 2 i ${INCDIR}/termios.h | grep -v compat | grep -v CHWFLOW >> ${.TARGET}

clean:
	rm -f Termios.txt ${DEFS}
