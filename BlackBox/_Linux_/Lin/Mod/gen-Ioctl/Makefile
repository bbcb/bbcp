# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

all: mkioctl Ioctl.odc

Ioctl.odc: Ioctl.txt
	./txt2odc.py Ioctl.txt Ioctl.odc

PY = python2.7
DEFS = defs-tiocm defs-ioctl

defs-ioctl: mkioctl
	./mkioctl > $@

Ioctl.txt: Ioctl.txt.templ ${DEFS} libver osname machine
	${PY} ./untempl.py Ioctl.txt.templ $@

defs-tiocm:
	./dumpdefs.py 2 0 s ${INCDIR}/i386-linux-gnu/bits/ioctl-types.h | grep TIOCM_ > $@

clean:
	rm -f mkioctl Ioctl.txt Ioctl.odc ${DEFS}
