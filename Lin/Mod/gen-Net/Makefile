# This is BSD Makefile
#    BSD       GNU
# $@    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

PY = python2.7

CFLAGS += -m32

DEFS = defs-basictypes defs-af defs-so defs-sol defs-ai defs-eai defs-somaxconn

all: Net.odc ${DEFS}

Net.odc: Net.txt
	./txt2odc.py Net.txt Net.odc

Net.txt: Net.txt.templ ${DEFS} libver osname machine fields-sockaddr fields-sockaddr_in fields-sockaddr_storage fields-addrinfo defs-fd_set-const defs-fd_set-type
	${PY} ./untempl.py Net.txt.templ $@

defs-basictypes: sizeofs
	./sizeofs > $@

defs-af:
	./dumpdefs.py 3 1 i ${INCDIR}/i386-linux-gnu/bits/socket.h | grep "	PF_" > $@

defs-sol:
	./dumpdefs.py 2 1 i ${INCDIR}/asm-generic/socket.h | grep "	SOL_" > $@

defs-somaxconn:
	./dumpdefs.py 2 1 i ${INCDIR}/i386-linux-gnu/bits/socket.h | grep "	SOMAXCONN" > $@

defs-so:
	./dumpdefs.py 3 1 s ${INCDIR}/asm-generic/socket.h | grep "	SO_" > $@

defs-ai:
	./dumpdefs.py 3 2 s ${INCDIR}/netdb.h | grep "	AI_" > $@

defs-eai:
	./dumpdefs.py 3 2 i ${INCDIR}/netdb.h | grep "	EAI_" > $@

clean:
	rm -f ${DEFS} Net.txt Net.odc sizeofs
