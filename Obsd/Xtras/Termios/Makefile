# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

all: Termios.odc

Termios.odc: Termios.txt
	./txt2odc.py Termios.txt Termios.odc

Termios.txt: Termios.txt.templ
	./untempl_h.py -m 32 Termios.txt.templ ${.TARGET}

clean:
	rm -f Termios.txt Termios.odc
