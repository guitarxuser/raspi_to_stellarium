
#DEBUG	= -O0 
DEBUG= -g
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lpthread -lm  -lrt


###############################################################################


all: gpiolib edge rot_enc devzero

gpiolib:
	$(CC) -c $(DEBUG) gpiolib.c -o gpiolib.o $(LDFLAGS) $(LDLIBS)
edge:	edge.o
	$(CC) -o $@ edge.o gpiolib.o $(LDFLAGS) $(LDLIBS)
rot_enc:	rot_enc.o	
	$(CC) -o $@ rot_enc.o gpiolib.o $(LDFLAGS) $(LDLIBS)
devzero:	devzero.o	
	$(CC) -o $@ devzero.o  $(LDFLAGS) $(LDLIBS)
clean:
	rm -f *.o


# DO NOT DELETE
