# Make File for building the Linux BlackBox Loader
# Runs on Linux ELF 

# for 32-bit
CFLAGS = -O2

# for 64-bit
# CFLAGS = -m32 -O2

exe.img:	blackbox.o
	gcc $(CFLAGS) -o exe.img blackbox.o -ldl -lm

blackbox.o:
	gcc $(CFLAGS) -c blackbox.c

clean:
	rm exe.img blackbox.o
