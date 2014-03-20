all: setitimer-helper squish-pty squish-unix
# 2038874749087570028

CC = gcc
CFLAGS = -Wall -W
LDFLAGS = -lm
setitimer-helper: setitimer-helper.o
squish-pty: squish-pty.o
squish-unix: squish-unix.o

clean: 
	rm -f *.o setitimer-helper squish-pty squish-unix
