# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

.PHONY: all clean

CFLAGS += -m32

DEFS = defs-basictypes

all: Net.odc ${DEFS}

%.odc: %.txt
	./txt2odc.py $< $@

Net.txt: Net.txt.templ ${DEFS} libver osname machine fields-sockaddr fields-sockaddr_in fields-sockaddr_storage fields-addrinfo defs-fd_set_const defs-fd_set_type
	./untempl_h.py -m 32 Net.txt.templ $@

defs-basictypes: sizeofs
	./sizeofs > $@

clean:
	rm -f ${DEFS} Net.txt Net.odc sizeofs
