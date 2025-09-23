# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

.PHONY: all clean

CFLAGS += -m32

DEFS = defs-basictypes

all: LibW.odc

%.odc: %.txt
	./txt2odc.py $< $@

LibW.txt: LibW.txt.templ ${DEFS} libver osname machine
	./untempl_h.py -m 32 LibW.txt.templ $@

defs-basictypes: sizeofs
	./sizeofs > $@

clean:
	rm -f ${DEFS} LibW.txt LibW.odc sizeofs
