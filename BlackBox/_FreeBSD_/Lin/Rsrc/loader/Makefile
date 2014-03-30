# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

CFLAGS += -Wall -O0 -g

all: loader

loader: loader.c
	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC}

clean:
	rm -f loader
