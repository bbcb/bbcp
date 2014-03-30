# This is GNU Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

all: loader

CFLAGS += -Wall -O0 -g -m32

loader: loader.c
	${CC} ${CFLAGS} -o $@ $^ -ldl

clean:
	rm -f loader
