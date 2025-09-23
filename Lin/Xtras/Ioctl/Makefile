# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

.PHONY: all clean

all: Ioctl.odc

%.odc: %.txt
	./txt2odc.py $< $@

Ioctl.txt: Ioctl.txt.templ libver osname machine
	./untempl_h.py -m 32 Ioctl.txt.templ $@

clean:
	rm -f Ioctl.txt Ioctl.odc
