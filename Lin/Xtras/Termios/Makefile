# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

.PHONY: all clean

all: Termios.odc

%.odc: %.txt
	./txt2odc.py $< $@

Termios.txt: Termios.txt.templ
	./untempl_h.py -m 32 $< $@

clean:
	rm -f Termios.txt Termios.odc
