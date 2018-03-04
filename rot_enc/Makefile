
DEBUG	= -O0 
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt
LDLIBS_GPIO= -lgpio  -lrt

###############################################################################


rot_enc:	rot_enc.o
	$(CC) -o $@ rot_enc.o $(LDFLAGS) $(LDLIBS)

clean:
	rm -f *.o


# DO NOT DELETE
