# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

INCDIR ?= /usr/include
INCDIR_I386 ?= ${INCDIR}/i386-linux-gnu

PY = python2.7

DEFS = clockspersec sigstksz defs-map defs-prot defs-sigmask nsig defs-signo defs-fpe defs-sa defs-errno namemax defs-fcntlo defs-basictypes defs-sc defs-clockid defs-wait

all: Libc.odc ${DEFS} strerrnocase.txt

Libc.odc: Libc.txt
	./txt2odc.py Libc.txt Libc.odc

Libc.txt: Libc.txt.templ ${DEFS} libver osname machine custom fields-dirent fields-sigaction fields-siginfo fields-stack fields-stat fields-tm fields-ucontext fields-timespec
	${PY} ./untempl.py Libc.txt.templ $@

clockspersec:
	grep "define CLOCKS_PER_SEC" ${INCDIR_I386}/bits/time.h | awk '{print $$4}' | cut -d ')' -f 1 | tr -d '\n' > $@

sigstksz:
	grep "define SIGSTKSZ" ${INCDIR}/asm-generic/signal.h | awk '{print $$3}' | tr -d '\n' > $@

defs-map:
	./dumpdefs.py 2 1 s ${INCDIR}/asm-generic/mman-common.h | grep "MAP_" > $@
	echo "		MAP_ANON* = MAP_ANONYMOUS;" >> $@

defs-prot:
	./dumpdefs.py 2 1 s ${INCDIR}/asm-generic/mman-common.h | grep "PROT_" > $@

defs-sigmask:
	./dumpdefs.py 2 2 i ${INCDIR}//asm-generic/signal-defs.h | grep "SIG_" | grep -v __sighandler_t > $@

defs-signo:
	./dumpdefs.py 2 1 i ${INCDIR}/asm-generic/signal.h | grep "	SIG" > $@

nsig:
	grep "#define _NSIG" ${INCDIR}/asm-generic/signal.h | head -1 | awk '{print $$3}' | tr -d '\n' > $@

defs-fpe:
	./dumpdefs.py 2 1 i ${INCDIR}/asm-generic/siginfo.h | grep "	FPE_" | sed s'/__SI_FAULT|//' > $@

defs-sa:
	./dumpdefs.py 2 1 s ${INCDIR}/asm-generic/signal.h | grep "	SA_" > $@

defs-errno:
	./dumpdefs.py 2 1 i ${INCDIR}/asm-generic/errno-base.h > $@
	./dumpdefs.py 2 1 i ${INCDIR}/asm-generic/errno.h >> $@

namemax:
	grep " NAME_MAX " ${INCDIR}/linux/limits.h | awk '{print $$3}' | tr -d '\n' > $@

defs-fcntlo:
	./dumpdefs.py 2 1 s ${INCDIR}/asm-generic/fcntl.h | grep "	O_" > $@
	./dumpdefs.py 2 2 s ${INCDIR}/asm-generic/fcntl.h | grep "	O_" >> $@

defs-basictypes: sizeofs
	./sizeofs > $@

defs-sc: scs
	./scs > $@

defs-clockid:
	./dumpdefs.py 2 1 i ${INCDIR}/linux/time.h | grep CLOCK_ > $@

defs-wait:
	grep "define WAIT_ANY" ${INCDIR_I386}/sys/wait.h | sed s'/(//' | sed s'/)//' | awk '{print "		" $$3 "* = " $$4 ";"}' > $@
	./dumpdefs.py 2 1 s ${INCDIR}/linux/wait.h | grep "	WCONTINUED" >> $@
	./dumpdefs.py 2 1 s ${INCDIR}/linux/wait.h | grep "	WNOHANG" >> $@
	./dumpdefs.py 2 1 s ${INCDIR}/linux/wait.h | grep "	WUNTRACED" >> $@

dumpstrerrno.c: defs-errno
	grep -v ERESTART $^ | grep -v EJUSTRETURN | ./mkdumpstrerrno.py > $@

strerrnocase.txt: dumpstrerrno
	./dumpstrerrno | ./mkstrerrnocase.py > $@

scs: scs.c
	${CC} -m32 scs.c -o $@

sizeofs: sizeofs.c
	${CC} -m32 sizeofs.c -o $@

clean:
	rm -f ${DEFS} Libc.txt Libc.odc dumpstrerrno dumpstrerrno.c strerrnocase.txt sizeofs scs
