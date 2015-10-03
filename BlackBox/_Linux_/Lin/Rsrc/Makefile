# Make File for building the Linux BlackBox Loader
# Runs on Linux ELF 

CFLAGS = -m32 -O2

all: exe.img

exe.img: blackbox.o
	${CC} $(CFLAGS) -o exe.img blackbox.o -ldl -lm

blackbox.o:	blackbox.c
	${CC} $(CFLAGS) -c blackbox.c

clean:
	rm -f exe.img blackbox.o
