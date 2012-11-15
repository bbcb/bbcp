# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

all: mkioctl Ioctl.txt

PY = python2.7
DEFS = defs-ioctl1 defs-ioctl

defs-ioctl: mkioctl
	./mkioctl > $@

Ioctl.txt: Ioctl.txt.templ ${DEFS}
	${PY} ./untempl.py Ioctl.txt.templ $@

defs-ioctl1:
	./dumpdefs.py 3 0 s /usr/include/i386-linux-gnu/bits/ioctl-types.h | grep TIOCM_ > $@

clean:
	rm -f mkioctl Ioctl.txt ${DEFS}
