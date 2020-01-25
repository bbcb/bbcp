# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

PY = python2.7

DEFS = defs-basictypes defs-lc

all: LibW.odc ${DEFS}

LibW.odc: LibW.txt
	./txt2odc.py LibW.txt LibW.odc

LibW.txt: LibW.txt.templ ${DEFS} libver osname machine
	${PY} ./untempl.py LibW.txt.templ ${.TARGET}

defs-basictypes: sizeofs
	./sizeofs > ${.TARGET}

defs-lc:
	./dumpdefs.py 2 1 i ${INCDIR}/locale.h | grep "	LC_" > ${.TARGET}

clean:
	rm -f ${DEFS} LibW.txt LibW.odc sizeofs
