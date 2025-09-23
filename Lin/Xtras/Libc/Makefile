# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

.PHONY: all clean

CFLAGS += -m32

DEFS = defs-errno defs-basictypes

all: Libc.odc ${DEFS} strerrnocase.txt

%.odc: %.txt
	./txt2odc.py $< $@

Libc.txt: Libc.txt.templ ${DEFS} libver osname machine custom fields-dirent fields-sigaction fields-siginfo fields-stack fields-stat fields-tm fields-ucontext fields-timespec
	./untempl_h.py -m 32 Libc.txt.templ $@

defs-errno.txt: defs-errno.txt.templ
	./untempl_h.py -m 32 defs-errno.txt.templ $@

defs-errno: defs-errno.txt
	cat $< | awk '{print $$1}' | sed s'/*//' > $@

defs-basictypes: sizeofs
	./sizeofs > $@

dumpstrerrno.c: defs-errno
	grep -v ERESTART $^ | grep -v EJUSTRETURN | ./mkdumpstrerrno.py > $@

strerrnocase.txt: dumpstrerrno
	./dumpstrerrno | ./mkstrerrnocase.py > $@

clean:
	rm -f ${DEFS} Libc.txt Libc.odc dumpstrerrno dumpstrerrno.c strerrnocase.txt sizeofs defs-errno.txt
