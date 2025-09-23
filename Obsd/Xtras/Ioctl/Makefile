# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

all: Ioctl.odc

Ioctl.odc: Ioctl.txt
	./txt2odc.py Ioctl.txt Ioctl.odc

Ioctl.txt: Ioctl.txt.templ libver osname machine
	./untempl_h.py -m 32 Ioctl.txt.templ ${.TARGET}

clean:
	rm -f Ioctl.txt Ioctl.odc
