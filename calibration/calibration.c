/**********************************************************************
* Filename    : calibration.c
* Description : 
* Author      :
* E-mail      : 
* website     : 
* Date        : 
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
#include <sys/mman.h>
#include <errno.h>
#include <termios.h>
#include <math.h>
#include <poll.h>


#define  RoAPin    0 /* gpio pin 11*/
#define  RoBPin    1 /* gpio pin 12 outside*/
#define  RoSPin    2
#define  RoCPin    3 /* gpio pin 15*/
#define  RoDPin    4 /* gpio pin 16 outside*/
#define  high  1
#define  low   0
#define  MAX_NUMBER 4294967296 /*maximal RA  2^32*/
#define  REVOL_STEPS_COUNT_RA 600 /*Encoder ra steps for 360 degree revolution*/
#define  REVOL_STEPS_COUNT_DEC 96  /*encoder dec steps for 360 degree revolution*/
#define gpio_MAXBUF 100
#define	SIZE		sizeof(long)	/* size of shared memory area */
static volatile int globalCounter=12;
static volatile int globalCounter_dec=24; /*polaris declination*/
static unsigned char flag_ra=high;

unsigned char Last_RoB_Status;
unsigned char Last_RoA_Status;
unsigned char Last_RoD_Status;
unsigned char Last_RoC_Status;
unsigned char Current_RoB_Status;
unsigned char Current_RoA_Status;
unsigned char Current_RoD_Status;
unsigned char Current_RoC_Status;
int fd;
struct timeval t1, t2;
long long t;
unsigned long microseconds = 500;

typedef struct{
	unsigned int ra;
	unsigned int dec;
}mmap_typ;

mmap_typ *map_addr;

/*local functions*/

int my_delay(unsigned long mikros);
int open_serial(char *);
int sendbytes(char * Buffer, int Count);
void send_calc_ra(int);
void send_calc_dec(int);
void rotaryClear(void);
void rotaryDeal_ra(void);
void rotaryDeal_dec(void);
void rotaryDeal_polaris_ra(void);


/***********************  main  *******************************/

int main(void)
{
	if(wiringPiSetup() < 0){
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}

	pinMode(RoAPin, INPUT);
	pinMode(RoBPin, INPUT);
	pinMode(RoCPin, INPUT);
	pinMode(RoDPin, INPUT);

	pullUpDnControl(RoAPin, PUD_UP);
	pullUpDnControl(RoBPin, PUD_UP);
	pullUpDnControl(RoCPin, PUD_UP);
	pullUpDnControl(RoDPin, PUD_UP);


			while(1){

				rotaryDeal_ra(); /*dummy simulation of rotatory encoder*/


}
}

/***********************  main end *******************************/



void rotaryDeal_ra(void)
{

	Last_RoB_Status = digitalRead(RoBPin);
	Last_RoA_Status = digitalRead(RoAPin);
	gettimeofday(&t1, NULL);
	while(digitalRead(RoAPin)){
		my_delay(microseconds);
		Current_RoB_Status = digitalRead(RoBPin);
		flag_ra = 1;
	}


	if(flag_ra == 1){
		flag_ra = 0;
		if((Last_RoB_Status == 1)&&(Current_RoB_Status == 0) ){
			globalCounter = globalCounter - 1;
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

			globalCounter = globalCounter + 1 ;
			/*
	printf("anti Last_RoB_Status : %d\n",Last_RoB_Status);
	printf("anti Current_RoB_Status : %d\n",Current_RoB_Status);
	printf("anti Last_RoA_Status : %d\n",Last_RoA_Status);
	printf("anti Current_RoA_Status : %d\n",Current_RoA_Status);
			 */
			printf("globalCounter : %d\n",globalCounter);

		}

				send_calc_ra(globalCounter);
	}
}






int my_delay(unsigned long mikros)
  {
  struct timespec ts;
  int resp;

  ts.tv_sec = mikros / 1000000L;
  ts.tv_nsec = (mikros % 1000000L) * 1000L;
  resp = nanosleep(&ts, (struct timespec *)NULL);
  return(resp);
  }


void send_calc_ra(int received_globalCounter)
{

  	float ra_to_send;
	int internal_counter;
	int revol_count=1;

        ra_to_send = 40000000; 
	internal_counter=received_globalCounter;

	revol_count= abs((int)(floor((float)received_globalCounter/REVOL_STEPS_COUNT_RA)));

	if(received_globalCounter >= REVOL_STEPS_COUNT_RA)
	{
		internal_counter=received_globalCounter - REVOL_STEPS_COUNT_RA*revol_count;
	}

	if(received_globalCounter < 0)
	{
		internal_counter=received_globalCounter + REVOL_STEPS_COUNT_RA*revol_count ;
	}

	ra_to_send = ((float)internal_counter/(float)REVOL_STEPS_COUNT_RA)*MAX_NUMBER;
		fprintf(stderr, "RA received_globalCounter=%d\n",received_globalCounter);
	fprintf(stderr, "internal_counter=%d\n",internal_counter);
	fprintf(stderr, "revol_count=%d\n",revol_count);
 
}



