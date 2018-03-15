#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "gpiolib.h"

#define  RoAPin    18 /*pin 12 green ch1 */
#define  RoBPin    17 /*pin 11 white ch2 */
#define  RoSPin    2

static volatile int globalCounter = 0 ;

unsigned char flag;
int Last_RoB_Status=0;
int Last_RoA_Status=0;
int Current_RoB_Status=0;
int loop = 1;

void rotaryDeal(void)

{         
        
	Last_RoB_Status = gpio_read(18);


	while((Last_RoA_Status = gpio_wait(17,500)) != 2 ){
               
		Current_RoB_Status = gpio_read(18) ;
		flag = 1;
		
	}

	fprintf (stderr,"flag is %d\n",flag);
		
	printf("clockw Last_RoB_Status : %d\n",Last_RoB_Status);
	printf("clockw Current_RoB_Status : %d\n",Current_RoB_Status);
	printf("clockw Last_RoA_Status : %d\n",Last_RoA_Status);
	/*	printf("clockw Current_RoA_Status : %d\n",Current_RoA_Status);*/
      

	if(flag == 1){
		flag = 0;
		if((Last_RoB_Status == 0)&&(Current_RoB_Status == 1)){
			globalCounter --;
			printf("globalCounter : %d\n",globalCounter);
		}
	
		if((Last_RoB_Status == 1)&&(Current_RoB_Status == 1)){
			globalCounter ++;
			printf("globalCounter : %d\n",globalCounter);
		}
	}
	
}



int main(void)

{
  /*
	if(wiringPiSetup() < 0){
		fprintf(stderr, "Unable to setup wiringPi:%s\n",strerror(errno));
		return 1;
	}
  */
  /* GPIO konfigurieren */
  
  if (gpio_export(17) != 0)
    {
      gpio_unexport(17);
      gpio_export(17);
    }
  gpio_direction(17, IN);
  gpio_edge(17, 'f');

  if( gpio_export(18)!=0);
  {
      gpio_unexport(18);
      gpio_export(18);  
  }
  gpio_direction(18, IN);
  gpio_edge(18, 'f');

  /*
	pinMode(RoAPin, INPUT);
	pinMode(RoBPin, INPUT);
	pinMode(RoSPin, INPUT);*/

  /*	pullUpDnControl(RoSPin, PUD_UP);*/

	while(1){
		rotaryDeal();
		/*	rotaryClear();*/
	}

	return 0;
}
