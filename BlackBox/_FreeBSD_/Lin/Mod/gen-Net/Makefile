# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

PY = python2.7

DEFS = defs-basictypes defs-af defs-sock defs-proto defs-shut defs-so defs-sol defs-msg defs-ai defs-eai defs-somaxconn

all: Net.odc ${DEFS}

Net.odc: Net.txt
	./txt2odc.py Net.txt Net.odc

Net.txt: Net.txt.templ ${DEFS} libver osname machine fields-sockaddr fields-sockaddr_in fields-sockaddr_storage fields-addrinfo defs-fd_set-const defs-fd_set-type
	${PY} ./untempl.py Net.txt.templ ${.TARGET}

defs-basictypes: sizeofs
	./sizeofs > ${.TARGET}

defs-af:
	./dumpdefs.py 3 1 i ${INCDIR}/sys/socket.h | grep "	AF_" > ${.TARGET}

defs-sock:
	./dumpdefs.py 3 1 i ${INCDIR}/sys/socket.h | grep "	SOCK_" > ${.TARGET}

defs-proto:
	./dumpdefs.py 3 1 i ${INCDIR}/netinet/in.h | grep "	IPPROTO_" > ${.TARGET}

defs-sol:
	./dumpdefs.py 2 1 i ${INCDIR}/sys/socket.h | grep "	SOL_" > ${.TARGET}

defs-msg:
	./dumpdefs.py 2 1 s ${INCDIR}/sys/socket.h | grep "	MSG_" > ${.TARGET}

defs-somaxconn:
	./dumpdefs.py 2 1 i ${INCDIR}/sys/socket.h | grep "	SOMAXCONN" > ${.TARGET}

defs-so:
	./dumpdefs.py 3 1 s ${INCDIR}/sys/socket.h | grep "	SO_" > ${.TARGET}

defs-shut:
	./dumpdefs.py 2 1 i ${INCDIR}/sys/socket.h | grep "	SHUT_" > ${.TARGET}

defs-ai:
	./dumpdefs.py 3 1 s ${INCDIR}/netdb.h | grep "	AI_" > ${.TARGET}

defs-eai:
	./dumpdefs.py 3 1 i ${INCDIR}/netdb.h | grep "	EAI_" > ${.TARGET}

clean:
	rm -f ${DEFS} Net.txt Net.odc sizeofs
