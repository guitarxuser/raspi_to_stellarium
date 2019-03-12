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
#define  REVOL_STEPS_COUNT 24  /*encoder steps for 360 degree revolution*/
#define gpio_MAXBUF 100
#define	SIZE		sizeof(long)	/* size of shared memory area */
static volatile int globalCounter = 0 ;
static volatile int globalCounter_dec = 0;
static unsigned char flag_ra=high;
static unsigned char flag_dec=high;
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
unsigned long microseconds = 5000;

typedef struct{
	unsigned int ra;
	unsigned int dec;
}mmap_typ;

mmap_typ *map_addr;

/*local functions*/

int my_delay(unsigned long mikros);


void send_calc_ra(int);
void send_calc_dec(int);
void rotaryClear(void);
void rotaryDeal_ra(void);
void rotaryDeal_dec(void);
void rotaryDeal_polaris_ra(void);

//static volatile float ra_to_send = 0;
//static volatile float dec_to_send = 0;


/***********************  main  *******************************/

int main(void)
{

	 int	fd_zero;
         int    i=0;
         pid_t  pid;
	 void	*area;


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
	     while(i < 10)
	     {
              rotaryDeal_polaris_ra();
	      i++;
	     }

	/* parent child game*/

	if ((fd_zero = open("/dev/zero", O_RDWR)) < 0)
		  fprintf(stderr,"open error\n");
		if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
		  fd_zero, 0)) == MAP_FAILED)
		  fprintf(stderr,"mmap error\n");

		map_addr=(mmap_typ*)area; /*shared memory parent and child */

		close(fd_zero);		/* can close /dev/zero now that it's mapped */


		if ((pid = fork()) < 0) {
			fprintf(stderr,"fork error\n");
		} else if (pid > 0) {			/* parent */


			fprintf(stderr,"I am parent\n");

			while(1){

				rotaryDeal_ra(); /*dummy simulation of rotatory encoder*/

			}
		}
		else {						/* child */
			fprintf(stderr,"I am child\n");

			while(1){

				rotaryDeal_dec(); /*dummy simulation of rotatory encoder*/

			}
		}

	return 0;
}

/***********************  main end *******************************/


void rotaryClear(void)
{
	if(digitalRead(RoSPin) == 0)
	{
		globalCounter = 0;
		printf("globalCounter : %d\n",globalCounter);
		delay(1000);
	}
}


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

void rotaryDeal_dec(void)
{


	Last_RoD_Status = digitalRead(RoDPin);
	Last_RoC_Status = digitalRead(RoCPin);

	while(digitalRead(RoCPin)){
		my_delay(microseconds);
		Current_RoD_Status = digitalRead(RoDPin);
		flag_dec = 1;
		//my_delay(500);
		break;
	}

	if(flag_dec == 1){
		flag_dec = 0;
		if((Last_RoD_Status == 1)&&(Current_RoD_Status == 0) ){
			globalCounter_dec = globalCounter_dec - 1;
			/*
	printf("clockw Last_RoB_Status : %d\n",Last_RoB_Status);
	printf("clockw Current_RoB_Status : %d\n",Current_RoB_Status);
	printf("clockw Last_RoA_Status : %d\n",Last_RoA_Status);
	printf("clockw Current_RoA_Status : %d\n",Current_RoA_Status);
			 */
			printf("globalCounter_dec : %d\n",globalCounter_dec);
		}
		if(Last_RoD_Status == 0 ){
			my_delay(500);
			Current_RoD_Status = digitalRead(RoDPin);
		}

		if((Last_RoD_Status == 0)&&(Current_RoD_Status == 1)){

			globalCounter_dec = globalCounter_dec + 1 ;
			/*
	printf("anti Last_RoB_Status : %d\n",Last_RoB_Status);
	printf("anti Current_RoB_Status : %d\n",Current_RoB_Status);
	printf("anti Last_RoA_Status : %d\n",Last_RoA_Status);
	printf("anti Current_RoA_Status : %d\n",Current_RoA_Status);
			 */
			printf("globalCounter_dec : %d\n",globalCounter_dec);


		}
	    send_calc_dec(globalCounter_dec);
	}

}

void rotaryDeal_polaris_ra(void)
{

	char  telsscope_telegram [18]; /*8char;8char#*/
	char* t_tele_p;
	float ra_to_send=0;

	t_tele_p=telsscope_telegram;



		ra_to_send=((float)globalCounter/(float)REVOL_STEPS_COUNT)*MAX_NUMBER;

		fprintf(stderr, "globalCounter=%d\n",globalCounter);
		if(ra_to_send ==0)
		{
		    fprintf(stderr,"%s,%s#\n","00000000","40000000");
		    sprintf(t_tele_p,"%s,%s#","00000000","40000000");
		}
		else
		{
		  fprintf(stderr,"%X,%s#\n",(unsigned int)ra_to_send,"40000000");
		  sprintf(t_tele_p,"%X,%s#",(unsigned int)ra_to_send,"40000000");

		}
		sleep(1);

		globalCounter = globalCounter + 1;

		 if (globalCounter ==  REVOL_STEPS_COUNT)
			{
				globalCounter=0;
				sleep(1);
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
	char  telsscope_telegram [18]; /*8char;8char#*/
	char* t_tele_p;
	float ra_to_send=40000000;
	int internal_counter;
	int revol_count=1;

	t_tele_p=telsscope_telegram;
	internal_counter=received_globalCounter;

	revol_count= abs((int)(floor((float)received_globalCounter/REVOL_STEPS_COUNT)));

	if(received_globalCounter >= REVOL_STEPS_COUNT)
	{
		internal_counter=received_globalCounter - REVOL_STEPS_COUNT*revol_count;
	}

	if(received_globalCounter < 0)
	{
		internal_counter=received_globalCounter + REVOL_STEPS_COUNT*revol_count ;
	}
	ra_to_send=((float)internal_counter/(float)REVOL_STEPS_COUNT)*MAX_NUMBER;
	fprintf(stderr, "received_globalCounter=%d\n",received_globalCounter);
	fprintf(stderr, "internal_counter=%d\n",internal_counter);
	fprintf(stderr, "revol_count=%d\n",revol_count);
        map_addr->ra=(unsigned int)ra_to_send;

		fprintf(stderr,"%X,%X#\n", map_addr->ra ,map_addr->dec);
		sprintf(t_tele_p,"%X,%X#", map_addr->ra ,map_addr->dec);


}

void send_calc_dec(int received_globalCounter)
{
	char  telsscope_telegram [18]; /*8char;8char#*/
	char* t_tele_p;
	float dec_to_send=95555500;
	int internal_counter;
	int revol_count=1;

	t_tele_p=telsscope_telegram;
	internal_counter=received_globalCounter;

	revol_count= abs((int)(floor((float)received_globalCounter/REVOL_STEPS_COUNT)));

	if(received_globalCounter >= REVOL_STEPS_COUNT)
	{
		internal_counter=received_globalCounter - REVOL_STEPS_COUNT*revol_count;
	}

	if(received_globalCounter < 0)
	{
		internal_counter=received_globalCounter + REVOL_STEPS_COUNT*revol_count ;
	}
	dec_to_send=((float)internal_counter/(float)REVOL_STEPS_COUNT)*MAX_NUMBER;
		fprintf(stderr, "received_globalCounter=%d\n",received_globalCounter);
	fprintf(stderr, "internal_counter=%d\n",internal_counter);
	fprintf(stderr, "revol_count=%d\n",revol_count);
        map_addr->dec=(unsigned int)dec_to_send;

		fprintf(stderr,"%X,%X#\n", map_addr->ra ,map_addr->dec);
		sprintf(t_tele_p,"%X,%X#", map_addr->ra ,map_addr->dec);

}

