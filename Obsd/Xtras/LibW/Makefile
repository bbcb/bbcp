.PHONY: all clean

CFLAGS += -m32

all: LibW.odc

LibW.odc: LibW.txt
	./txt2odc.py LibW.txt LibW.odc

LibW.txt: LibW.txt.templ defs-basictypes subsystem libver osname machine custom
	./untempl_h.py -m 32 LibW.txt.templ LibW.txt

defs-basictypes: sizeofs
	./sizeofs > defs-basictypes

clean:
	rm -f LibW.txt LibW.odc sizeofs defs-basictypes
