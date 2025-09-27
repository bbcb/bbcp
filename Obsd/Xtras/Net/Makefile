.PHONY: all clean

CFLAGS += -m32

DEFS = defs-basictypes

all: Net.odc

Net.odc: Net.txt
	./txt2odc.py Net.txt Net.odc

Net.txt: Net.txt.templ ${DEFS} subsystem libver osname machine fields-sockaddr fields-sockaddr_in fields-sockaddr_storage fields-addrinfo defs-fd_set_const defs-fd_set_type
	./untempl_h.py -m 32 Net.txt.templ Net.txt

defs-basictypes: sizeofs
	./sizeofs > defs-basictypes

clean:
	rm -f ${DEFS} Net.txt Net.odc sizeofs
