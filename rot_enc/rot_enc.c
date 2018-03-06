/**********************************************************************
* Filename    : rot_enc.c derived by rotaryEncoder.c
* Description : make a rotaryEncoder work.
* Author      : Robot
* E-mail      : support@sunfounder.com
* website     : www.sunfounder.com
* Date        : 2014/08/27
**********************************************************************/
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <math.h>


#define  RoAPin    0 /* gpio pin 11*/
#define  RoBPin    1 /* gpio pin 12 outside*/
#define  RoSPin    2 
#define  high  1
#define  low   0
#define  MAX_NUMBER 4294967296 /*maximal RA  2^32*/
#define  REVOL_STEPS_COUNT 24  /*encoder steps for 360 degree revolution*/
static volatile int globalCounter = 0 ;

unsigned char flag=high;
unsigned char Last_RoB_Status;
unsigned char Last_RoA_Status;
unsigned char Current_RoB_Status;
unsigned char Current_RoA_Status;
int fd;                    /* Filedeskriptor */
struct timeval t1, t2;
long long t;
unsigned long microseconds = 5000;

/*local functions*/

int my_delay(unsigned long mikros);
int open_serial(char *);
int sendbytes(char * Buffer, int Count);
void send_calc_ra(int);
/*
The byte order for all kind of integers is always
"least significiant byte first".

-----------------------
server->client:
MessageCurrentPosition (type = 0):

LENGTH (2 bytes,integer): length of the message
TYPE   (2 bytes,integer): 0
TIME   (8 bytes,integer): current time on the server computer in microseconds
           since 1970.01.01 UT. Currently unused.
RA     (4 bytes,unsigned integer): right ascension of the telescope (J2000)
           a value of 0x100000000 = 0x0 means 24h=0h,
           a value of 0x80000000 means 12h
DEC    (4 bytes,signed integer): declination of the telescope (J2000)
           a value of -0x40000000 means -90degrees,
           a value of 0x0 means 0degrees,
           a value of 0x40000000 means 90degrees
STATUS (4 bytes,signed integer): status of the telescope, currently unused.
           status=0 means ok, status<0 means some error
           */



struct stellarium_protocol{
	short int length;
	short int type;
	long int time;
	int ra;
	int dec;
	int status;
}st_p;
//typedef stellarium_protocol struct stellarium_protocol;

void rotaryDeal(void)
{
  
  Last_RoB_Status = digitalRead(RoBPin);
  Last_RoA_Status = digitalRead(RoAPin);
  gettimeofday(&t1, NULL);
  while(digitalRead(RoAPin)){
    my_delay(microseconds);               
    Current_RoB_Status = digitalRead(RoBPin);
    flag = 1;
  }
  /*
    gettimeofday(&t2, NULL);
    t = ((t2.tv_sec * 1000000) + t2.tv_usec) - ((t1.tv_sec * 1000000) + t1.tv_usec);
    printf("Aufruf von while(digitalRead(RoAPin)   dauerte  %lld us\n", t);
  */
      
  if(flag == 1){
    flag = 0;
    if((Last_RoB_Status == 1)&&(Current_RoB_Status == 0) ){
      globalCounter = globalCounter + 1;
      /*	
	printf("clockw Last_RoB_Status : %d\n",Last_RoB_Status);
	printf("clockw Current_RoB_Status : %d\n",Current_RoB_Status);
	printf("clockw Last_RoA_Status : %d\n",Last_RoA_Status);
	printf("clockw Current_RoA_Status : %d\n",Current_RoA_Status);
      */
      printf("globalCounter : %d\n",globalCounter);
      send_calc_ra(globalCounter);
    }
    if(Last_RoB_Status == 0 ){
      my_delay(500);
      Current_RoB_Status = digitalRead(RoBPin);
    }
               
    if((Last_RoB_Status == 0)&&(Current_RoB_Status == 1)){
 
      globalCounter = globalCounter - 1 ;
      /*  
	printf("anti Last_RoB_Status : %d\n",Last_RoB_Status);
	printf("anti Current_RoB_Status : %d\n",Current_RoB_Status);
	printf("anti Last_RoA_Status : %d\n",Last_RoA_Status);
	printf("anti Current_RoA_Status : %d\n",Current_RoA_Status);
      */
      printf("globalCounter : %d\n",globalCounter);
    
      send_calc_ra(globalCounter);
    }	
  }
}


void rotaryClear(void)
{
	if(digitalRead(RoSPin) == 0)
	{
		globalCounter = 0;
		printf("globalCounter : %d\n",globalCounter);
		delay(1000);
	}
}


/***********************main*******************************/
int main(int argc, char* argv[])
{
 
	char* device=argv[1];

	if (argc <2)
	{
		fprintf(stderr,"please call rot_enc </dev/pts/number>\n");
		return(-1);
	}

	if(open_serial(device) < 0 ) /*and fd within it*/
	  {
           fprintf(stderr, "Unable to setup device:%s\n",strerror(errno));
           return 1;
          }

	if(wiringPiSetup() < 0){
	    fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
	    return 1;
	}

	pinMode(RoAPin, INPUT);
	pinMode(RoBPin, INPUT);
/*		pinMode(RoSPin, INPUT);

		pullUpDnControl(RoSPin, PUD_UP);*/
	pullUpDnControl(RoAPin, PUD_UP);
	pullUpDnControl(RoBPin, PUD_UP);
        while(1){
	 
	  /*		rotaryDeal_dummy();*/

	  rotaryDeal();
		
		/*   	rotaryClear();*/
	}
        close(fd);
	return 0;
}

/***********************main*******************************/

int my_delay(unsigned long mikros)
  {
  struct timespec ts;
  int resp;

  ts.tv_sec = mikros / 1000000L;
  ts.tv_nsec = (mikros % 1000000L) * 1000L;
  resp = nanosleep(&ts, (struct timespec *)NULL);
  return(resp);
  }

int open_serial(char *device)
  {
  /*
   * opens serial port
   *
   * RS232-Parameter:
   * 9600 bps, 8 data, 1 stopbit, no parity, no handshake
   */

   /*int fd;*/                    
   struct termios options;    

   /* open port - read/write, no "controlling tty", ignore status von DCD  */
   fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
   if (fd >= 0)
     {
     /* get the current options */
     fcntl(fd, F_SETFL, 0);
     if (tcgetattr(fd, &options) != 0) return(-1);
     memset(&options, 0, sizeof(options)); 

     /* set baudrate */
     cfsetispeed(&options, B9600);
     cfsetospeed(&options, B9600);

     /* setze Optionen */
     options.c_cflag &= ~PARENB;         /* no parity */
     options.c_cflag &= ~CSTOPB;         /* 1 stopbit */
     options.c_cflag &= ~CSIZE;          /* 8 databits */
     options.c_cflag |= CS8;

     /* 9600 bps, 8 data, ignore CD-Signal , allow read */
     options.c_cflag |= (CLOCAL | CREAD);

     /* no echo, no control character, no interrupts */
     options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
     options.c_iflag = IGNPAR;           /* ignore parity error */
     options.c_oflag &= ~OPOST;          /* set "raw" input */
     options.c_cc[VMIN]  = 0;            /* wait for min. 0 characters */
     options.c_cc[VTIME] = 10;           /* timeout 1 sec */
     tcflush(fd,TCIOFLUSH);              /* empty buffer */
     if (tcsetattr(fd, TCSAFLUSH, &options) != 0) return(-1);

     }
       
  return(fd);
  }


int sendbytes(char * Buffer, int Count)
/* Sendet Count Bytes aus dem Puffer Buffer */
  {
  int sent;  /* return-Wert */
  /*  Daten senden */
  sent = write(fd, Buffer, Count);
  if (sent < 0)
    {
    perror("sendbytes failed - error!");
    return -1;
    }
  if (sent < Count) 
    { 
    perror("sendbytes failed - truncated!");
    }
  return sent;
  }

void send_calc_ra(int received_globalCounter)
{
	char  telsscope_telegram [18]; /*8char;8char#*/
	char* t_tele_p;
	float ra_to_send=0;
        int internal_counter;
        int revol_count=1;

	t_tele_p=telsscope_telegram;
        internal_counter=received_globalCounter;

        revol_count= abs((int)(floor((float)received_globalCounter/REVOL_STEPS_COUNT)));

        if(received_globalCounter >= 24)
	  {
	    internal_counter=received_globalCounter - REVOL_STEPS_COUNT*revol_count;
          }
 
         if(received_globalCounter < 0)
	  {
	    internal_counter=received_globalCounter + REVOL_STEPS_COUNT*revol_count ;
          }
 	ra_to_send=((float)internal_counter/(float)24)*MAX_NUMBER;
	fprintf(stderr, "received_globalCounter=%d\n",received_globalCounter);
	fprintf(stderr, "internal_counter=%d\n",internal_counter);
	fprintf(stderr, "revol_count=%d\n",revol_count);
        if(ra_to_send != 0)
	  {
	   fprintf(stderr,"%X,%s#\n",(unsigned int)ra_to_send,"20670280");
	   sprintf(t_tele_p,"%X,%s#",(unsigned int)ra_to_send,"20670280");
	  }
        if(ra_to_send == 0)
	  {
	   fprintf(stderr,"%s,%s#\n","00000000","20670280");
	   sprintf(t_tele_p,"%s,%s#","00000000","20670280");
	  }
	sendbytes(t_tele_p, strlen(t_tele_p));
}
