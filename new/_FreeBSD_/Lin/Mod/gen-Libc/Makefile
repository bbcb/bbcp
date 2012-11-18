# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

PY = python2.7

DEFS = clockspersec defs-map defs-prot defs-sigmask defs-signo defs-fpe defs-sa defs-errno namemax defs-fcntlo defs-basictypes defs-sc

all: Libc.txt ${DEFS} strerrnocase.txt

Libc.txt: Libc.txt.templ ${DEFS} libver osname machine custom fields-dirent fields-sigaction fields-siginfo fields-stack fields-stat fields-tm fields-ucontext
	${PY} ./untempl.py Libc.txt.templ ${.TARGET}

clockspersec: /usr/include/time.h
	grep CLOCKS_PER_SEC ${.ALLSRC} | head -1 | awk '{print $$3}' | tr -d '\n' > ${.TARGET}

defs-map: /usr/include/sys/mman.h
	./dumpdefs.py 2 1 s ${.ALLSRC} | grep "	MAP_" > ${.TARGET}
	./dumpdefs.py 2 2 s ${.ALLSRC} | grep "	MAP_" >> ${.TARGET}

defs-prot: /usr/include/sys/mman.h
	./dumpdefs.py 2 1 s ${.ALLSRC} | grep "	PROT_" > ${.TARGET}

defs-sigmask: /usr/include/sys/signal.h
	./dumpdefs.py 2 2 i ${.ALLSRC} | grep "	SIG_" > ${.TARGET}

defs-signo: /usr/include/sys/signal.h
	./dumpdefs.py 2 1 i ${.ALLSRC} | grep "	SIG" > ${.TARGET}
	./dumpdefs.py 2 2 i ${.ALLSRC} | grep "	SIG" | grep -v SIG_ >> ${.TARGET}

defs-fpe: /usr/include/sys/signal.h
	./dumpdefs.py 2 2 i ${.ALLSRC} | grep "	FPE_" > ${.TARGET}

defs-sa: /usr/include/sys/signal.h
	./dumpdefs.py 2 2 s ${.ALLSRC} | grep "	SA_" > ${.TARGET}

defs-errno: /usr/include/sys/errno.h
	./dumpdefs.py 2 0 i ${.ALLSRC} > ${.TARGET}
	./dumpdefs.py 2 1 i ${.ALLSRC} >> ${.TARGET}
	./dumpdefs.py 2 2 i ${.ALLSRC} >> ${.TARGET}

namemax: /usr/include/sys/syslimits.h
	grep "	NAME_MAX	" ${.ALLSRC} | head -1 | awk '{print $$3}' | tr -d '\n' > ${.TARGET}

defs-fcntlo: /usr/include/fcntl.h
	./dumpdefs.py 2 2 s ${.ALLSRC} | grep -v compat | grep "	O_" > ${.TARGET}
	./dumpdefs.py 2 1 s ${.ALLSRC} | grep "	O_" >> ${.TARGET}

defs-basictypes: sizeofs
	./sizeofs > ${.TARGET}

defs-sc: /usr/include/unistd.h
	./dumpdefs.py 2 1 i ${.ALLSRC} | grep _SC_ > ${.TARGET}
	./dumpdefs.py 2 2 i ${.ALLSRC} | grep _SC_ > ${.TARGET}

#defs-madv:
#	./dumpdefs.py 2 1 i /usr/include/sys/mman.h | grep MADV | sed 's/_//' > ${.TARGET}
#	./dumpdefs.py 2 2 i /usr/include/sys/mman.h | grep MADV >> ${.TARGET}
#
#defs-signal1:
#	./dumpdefs.py 2 2 i /usr/include/sys/signal.h | grep ILL_ >> ${.TARGET}
#	./dumpdefs.py 2 2 i /usr/include/sys/signal.h | grep SEGV_ >> ${.TARGET}
#	./dumpdefs.py 2 2 i /usr/include/sys/signal.h | grep BUS_ >> ${.TARGET}

dumpstrerrno.c: defs-errno
	grep -v ERESTART ${.ALLSRC} | grep -v EJUSTRETURN | grep -v ENOIOCTL | grep -v EDIRIOCTL | ./mkdumpstrerrno.py > ${.TARGET}

strerrnocase.txt: dumpstrerrno
	./dumpstrerrno | ./mkstrerrnocase.py > ${.TARGET}

clean:
	rm -f ${DEFS} Libc.txt dumpstrerrno dumpstrerrno.c strerrnocase.txt sizeofs
