# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

PY = python2.7

CFLAGS += -m32

DEFS = defs-basictypes defs-lc

all: LibW.odc ${DEFS}

LibW.odc: LibW.txt
	./txt2odc.py LibW.txt LibW.odc

LibW.txt: LibW.txt.templ ${DEFS} libver osname machine
	${PY} ./untempl.py LibW.txt.templ $@

defs-basictypes: sizeofs
	./sizeofs > $@

defs-lc: mklc
	./mklc > $@

clean:
	rm -f ${DEFS} LibW.txt LibW.odc sizeofs mklc
