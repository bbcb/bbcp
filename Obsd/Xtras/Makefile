SUBDIRS = Ioctl LibW Libc Net Termios

.PHONY: all clean ${SUBDIRS}

all: ${SUBDIRS}

${SUBDIRS}:
	${MAKE} -C $@

clean:
	for subdir in ${SUBDIRS}; do \
		${MAKE} -C $${subdir} clean ; \
	done
