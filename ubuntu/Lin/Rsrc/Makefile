# Make File for building the Linux BlackBox Loader
# Runs on Linux ELF 

CFLAGS = -O2

exe.img:	blackbox.o
	gcc $(CFLAGS) -o exe.img blackbox.o -ldl -lm

blackbox.o:
	gcc $(CFLAGS) -c blackbox.c

clean:
	rm exe.img blackbox.o
