# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

all: mkioctl Ioctl.txt

PY = python2.7
DEFS = defs-ttycom defs-ioctl

defs-ioctl: mkioctl
	./mkioctl > ${.TARGET}

Ioctl.txt: Ioctl.txt.templ ${DEFS}
	${PY} ./untempl.py Ioctl.txt.templ ${.TARGET}

defs-ttycom:
	./dumpdefs.py 3 1 s ${INCDIR}/sys/ttycom.h | grep TIOCM_ > ${.TARGET}

clean:
	rm -f mkioctl Ioctl.txt ${DEFS}
