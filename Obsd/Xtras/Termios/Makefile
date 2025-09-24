CFLAGS += -m32

all: Termios.odc

Termios.odc: Termios.txt
	./txt2odc.py Termios.txt Termios.odc

Termios.txt: Termios.txt.templ defs-basictypes fields-termios subsystem libver osname machine
	./untempl_h.py -m 32 Termios.txt.templ Termios.txt

defs-basictypes: sizeofs
	./sizeofs > defs-basictypes

clean:
	rm -f Termios.txt Termios.odc sizeofs defs-basictypes
