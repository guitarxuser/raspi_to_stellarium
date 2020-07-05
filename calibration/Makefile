
DEBUG	= -O0 
CC	= gcc
INCLUDE	= -I/usr/local/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L/usr/local/lib
LDLIBS    = -lwiringPi -lwiringPiDev -lpthread -lm -lcrypt -lrt
LDLIBS_GPIO= -lgpio  -lrt

###############################################################################


all: calibration 

calibration:	calibration.o
	$(CC) -o $@ calibration.o $(LDFLAGS) $(LDLIBS)
clean:
	rm -f *.o


# DO NOT DELETE
