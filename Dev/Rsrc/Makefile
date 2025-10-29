# use GNU make

.PHONY: all clean

CFLAGS += -m32 -Werror -O2 -pipe
IMG = exe$(shell uname -s).img
SRC = bbrun.c

STRIP ?= strip
STRIPFLAGS ?= -s

all: ${IMG}

${IMG}: ${SRC}
	@SIZE0=0; \
	SIZE=30000; \
	while [ $$SIZE0 -ne $$SIZE ]; do \
	    echo "Compiling with EXESIZE=$$SIZE"; \
	    ${CC} ${CFLAGS} -DEXESIZE=$$SIZE -o "${IMG}" "${SRC}"; \
	    ${STRIP} ${STRIPFLAGS} "${IMG}"; \
	    SIZE0=$$SIZE; \
	    SIZE=$$(wc -c "${IMG}" | awk '{print $$1}'); \
	done; \
	echo "Final size: $$SIZE"

clean:
	rm -f ${IMG}
