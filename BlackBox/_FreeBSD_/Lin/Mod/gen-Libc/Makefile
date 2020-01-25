# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include

PY = python2.7

DEFS = clockspersec defs-map defs-prot defs-sigmask defs-signo defs-fpe defs-sa defs-errno namemax defs-fcntlo defs-basictypes defs-sc

all: Libc.odc ${DEFS} strerrnocase.txt

Libc.odc: Libc.txt
	./txt2odc.py Libc.txt Libc.odc

Libc.txt: Libc.txt.templ ${DEFS} libver osname machine custom fields-dirent fields-sigaction fields-siginfo fields-stack fields-stat fields-tm fields-ucontext
	${PY} ./untempl.py Libc.txt.templ ${.TARGET}

clockspersec:
	grep CLOCKS_PER_SEC ${INCDIR}/time.h | head -1 | awk '{print $$3}' | tr -d '\n' > ${.TARGET}

defs-map:
	./dumpdefs.py 2 1 s ${INCDIR}/sys/mman.h | grep "	MAP_" > ${.TARGET}
	./dumpdefs.py 2 2 s ${INCDIR}/sys/mman.h | grep "	MAP_" | grep -v " = MAP_" >> ${.TARGET}

defs-prot:
	./dumpdefs.py 2 1 s ${INCDIR}/sys/mman.h | grep "	PROT_" > ${.TARGET}

defs-sigmask:
	./dumpdefs.py 2 2 i ${INCDIR}/sys/signal.h | grep "	SIG_" > ${.TARGET}

defs-signo:
	./dumpdefs.py 2 1 i ${INCDIR}/sys/signal.h | grep "	SIG" > ${.TARGET}
	./dumpdefs.py 2 2 i ${INCDIR}/sys/signal.h | grep "	SIG" | grep -v SIG_ >> ${.TARGET}

defs-fpe:
	./dumpdefs.py 2 2 i ${INCDIR}/sys/signal.h | grep "	FPE_" > ${.TARGET}

defs-sa:
	./dumpdefs.py 2 2 s ${INCDIR}/sys/signal.h | grep "	SA_" > ${.TARGET}

defs-errno:
	./dumpdefs.py 2 0 i ${INCDIR}/sys/errno.h > ${.TARGET}
	./dumpdefs.py 2 1 i ${INCDIR}/sys/errno.h >> ${.TARGET}
	./dumpdefs.py 2 2 i ${INCDIR}/sys/errno.h | grep -v "= -" >> ${.TARGET}

namemax:
	grep "	NAME_MAX	" ${INCDIR}/sys/syslimits.h | head -1 | awk '{print $$3}' | tr -d '\n' > ${.TARGET}

defs-fcntlo:
	./dumpdefs.py 2 2 s ${INCDIR}/fcntl.h | grep -v compat | grep "	O_" > ${.TARGET}
	./dumpdefs.py 2 1 s ${INCDIR}/fcntl.h | grep "	O_" >> ${.TARGET}

defs-basictypes: sizeofs
	./sizeofs > ${.TARGET}

defs-sc:
	./dumpdefs.py 2 1 i ${INCDIR}/unistd.h | grep _SC_ > ${.TARGET}
	./dumpdefs.py 2 2 i ${INCDIR}/unistd.h | grep _SC_ > ${.TARGET}

#defs-madv:
#	./dumpdefs.py 2 1 i ${INCDIR}/sys/mman.h | grep MADV | sed 's/_//' > ${.TARGET}
#	./dumpdefs.py 2 2 i ${INCDIR}/sys/mman.h | grep MADV >> ${.TARGET}
#
#defs-signal1:
#	./dumpdefs.py 2 2 i ${INCDIR}/sys/signal.h | grep ILL_ >> ${.TARGET}
#	./dumpdefs.py 2 2 i ${INCDIR}/sys/signal.h | grep SEGV_ >> ${.TARGET}
#	./dumpdefs.py 2 2 i ${INCDIR}/sys/signal.h | grep BUS_ >> ${.TARGET}

dumpstrerrno.c: defs-errno
	grep -v ERESTART ${.ALLSRC} | grep -v EJUSTRETURN | grep -v ENOIOCTL | grep -v EDIRIOCTL | ./mkdumpstrerrno.py > ${.TARGET}

strerrnocase.txt: dumpstrerrno
	./dumpstrerrno | ./mkstrerrnocase.py > ${.TARGET}

clean:
	rm -f ${DEFS} Libc.txt Libc.odc dumpstrerrno dumpstrerrno.c strerrnocase.txt sizeofs
