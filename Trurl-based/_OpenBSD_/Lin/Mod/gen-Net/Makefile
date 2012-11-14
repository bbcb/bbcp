# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

all: Net.txt dumphstrerrno

PY = python2.7
DEFS = defs-socket defs-in defs-netdb

Net.txt: Net.txt.templ ${DEFS}
	${PY} ./untempl.py Net.txt.templ ${.TARGET}

defs-socket:
	./dumpdefs.py 3 1 i /usr/include/sys/socket.h | grep "	SOCK_" > ${.TARGET}
	./dumpdefs.py 3 1 i /usr/include/sys/socket.h | grep "	AF_" >> ${.TARGET}
	./dumpdefs.py 3 1 i /usr/include/sys/socket.h | grep "	SHUT_" >> ${.TARGET}
	./dumpdefs.py 3 1 i /usr/include/sys/socket.h | grep "	SOMAXCONN" >> ${.TARGET}
	./dumpdefs.py 3 1 s /usr/include/sys/socket.h | grep "	SO_" >> ${.TARGET}
	./dumpdefs.py 3 1 i /usr/include/sys/socket.h | grep "	SOL_" >> ${.TARGET}
	./dumpdefs.py 3 1 s /usr/include/sys/socket.h | grep "	MSG_" >> ${.TARGET}

defs-in:
	./dumpdefs.py 3 1 i /usr/include/netinet/in.h | grep "	IPPROTO_" > ${.TARGET}

defs-netdb:
	./dumpdefs.py 3 2 i /usr/include/netdb.h > ${.TARGET}

clean:
	rm -f Net.txt ${DEFS} dumphstrerrno
