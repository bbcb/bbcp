# This is BSD Makefile
#    BSD       GNU
# ${.TARGET}    $@
# ${.ALLSRC}    $^
# ${.IMPSRC}    $<

CFLAGS += -Wall -O0 -g

all: libdlobsdwrap.so dev0 BlackBox

# libdlobsdwrap.so: universal method of correct access to dl* functions
#	in case of dynamic loading of libBB*.so "ld.so" in LibDl may be used instead of "libc.so", but not in case of static linking
#	wrapper method is universal

# -pthread required to dlopen libraries that depends on pthread

#BlackBox: BlackBox.c
#	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC} -L. -lBB -pthread

BlackBox: BlackBox-dl.c
	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC} -pthread

#dev0: BlackBox1.c
#	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC} -L . -lBB0

dev0: dev0.c
	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC}

libdlobsdwrap.so: libdlobsdwrap.c
	${CC} ${CFLAGS} -o ${.TARGET} ${.ALLSRC} -fPIC -shared

clean:
	rm -f dev0 BlackBox libdlobsdwrap.so
