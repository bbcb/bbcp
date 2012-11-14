# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

PY = python2.7

DEFS = defs-errno defs-signal defs-sc defs-madv defs-siginfo defs-siginfo1 defs-sa defs-mman defs-fcntl defs-sig

all: Libc.txt ${DEFS}

Libc.txt: Libc.txt.templ ${DEFS} sizeofs
	${PY} ./untempl.py Libc.txt.templ ${.TARGET}
#	perl -pe 's/\n/\r\n/' < ${.TARGET}.tmp > ${.TARGET}
#	${PY} ./untempl2.py ${.TARGET}.tmp ${.TARGET}
#	rm -f ${.TARGET}.tmp

sizeofs: sizeofs.c
	${CC} -O0 -g -o ${.TARGET} ${.ALLSRC}

defs-errno:
	./dumpdefs.py 2 0 i /usr/include/sys/errno.h > ${.TARGET}
	./dumpdefs.py 2 1 i /usr/include/sys/errno.h >> ${.TARGET}

defs-tftp:
	./dumpdefs.py 2 1 i /usr/include/arpa/tftp.h | grep EACCESS > ${.TARGET}

defs-signal:
	./dumpdefs.py 2 1 i /usr/include/sys/signal.h > ${.TARGET}
	./dumpdefs.py 2 2 i /usr/include/sys/signal.h | grep SIGWINCH >> ${.TARGET}
	./dumpdefs.py 2 2 i /usr/include/sys/signal.h | grep SIGTHR >> ${.TARGET}

defs-sig:
	./dumpdefs.py 2 2 i /usr/include/sys/signal.h | grep SIG_ > ${.TARGET}

defs-sa:
	./dumpdefs.py 2 2 s /usr/include/sys/signal.h | grep "	SA_" > ${.TARGET}
	./dumpdefs.py 2 3 s /usr/include/sys/signal.h | grep "	SA_" >> ${.TARGET}

defs-mman:
	./dumpdefs.py 2 0 s /usr/include/sys/mman.h | grep "	PROT_" > ${.TARGET}
	./dumpdefs.py 2 0 s /usr/include/sys/mman.h | grep "	MAP_" >> ${.TARGET}

defs-fcntl:
	./dumpdefs.py 2 2 s /usr/include/fcntl.h | grep -v compat | grep "	O_" > ${.TARGET}
	./dumpdefs.py 2 1 s /usr/include/fcntl.h | grep "	O_" >> ${.TARGET}

defs-siginfo:
	./dumpdefs.py 2 1 i /usr/include/sys/siginfo.h > ${.TARGET}
	./dumpdefs.py 2 2 i /usr/include/sys/siginfo.h >> ${.TARGET}

defs-sc:
	./dumpdefs.py 2 1 i /usr/include/unistd.h | grep _SC_ > ${.TARGET}

defs-madv:
	./dumpdefs.py 2 0 i /usr/include/sys/mman.h | grep MADV > ${.TARGET}
	./dumpdefs.py 2 1 i /usr/include/sys/mman.h | grep MADV >> ${.TARGET}

defs-siginfo1:
	./dumpdefs.py 2 2 i /usr/include/sys/siginfo.h | grep FPE_ > ${.TARGET}
	./dumpdefs.py 2 2 i /usr/include/sys/siginfo.h | grep ILL_ >> ${.TARGET}
	./dumpdefs.py 2 2 i /usr/include/sys/siginfo.h | grep SEGV_ >> ${.TARGET}
	./dumpdefs.py 2 2 i /usr/include/sys/siginfo.h | grep BUS_ >> ${.TARGET}

clean:
	rm -f sizeofs ${DEFS} Libc.txt
