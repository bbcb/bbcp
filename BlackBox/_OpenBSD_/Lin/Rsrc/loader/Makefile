# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

CFLAGS += -Wall -O0 -g

all: loader

# -pthread required to dlopen libraries that depends on pthread

loader: loader.c
	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC} -pthread -Wl,-z,'wxneeded'

clean:
	rm -f loader
