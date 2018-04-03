#include <signal.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>		/* for convenience */
#include <stddef.h>		/* for offsetof */
#include <string.h>		/* for convenience */
#include <unistd.h>		/* for convenience */
#define	NLOOPS		1000
#define	SIZE		sizeof(long)	/* size of shared memory area */
#define  MAX_NUMBER 4294967296 /*maximal RA  2^32*/
#define  REVOL_STEPS_COUNT 48  /*encoder steps for 360 degree revolution*/

typedef struct{
	unsigned int ra;
	unsigned int dec;
}mmap_typ;

mmap_typ *map_addr;

static volatile int globalCounter = 0 ;
void rotaryDeal_ra(void);
void rotaryDeal_dec(void);




int main(void)
{
    int	fd;
	pid_t	pid;
	void	*area;



	if ((fd = open("/dev/zero", O_RDWR)) < 0)
	  fprintf(stderr,"open error\n");
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
	  fd, 0)) == MAP_FAILED)
	  fprintf(stderr,"mmap error\n");

	map_addr=(mmap_typ*)area; /*shared memory parent and child */

	close(fd);		/* can close /dev/zero now that it's mapped */


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

	exit(0);
}

void rotaryDeal_ra(void)
{

	char  telsscope_telegram [18]; /*8char;8char#*/
	char* t_tele_p;
	float ra_to_send=0;

	t_tele_p=telsscope_telegram;



		ra_to_send=((float)globalCounter/(float)REVOL_STEPS_COUNT)*MAX_NUMBER;

		fprintf(stderr, "globalCounter=%d\n",globalCounter);
		/* sprintf(t_tele_p,"%s","64AB0500,40CE0500#");*/
		/*fprintf(stderr,"%X,%s#\n",(int)ra_to_send,"40CE0500");
                sprintf(t_tele_p,"%X,%s#",(int)ra_to_send,"40CE0500");*/

		if(ra_to_send ==0)
		{
		    fprintf(stderr,"%s,%s#\n","00000000","20670280");
		    sprintf(t_tele_p,"%s,%s#","00000000","20670280");
		}
		else
		{
	//	fprintf(stderr,"%X,%s#\n",(unsigned int)ra_to_send,"20670280");
	//	sprintf(t_tele_p,"%X,%s#",(unsigned int)ra_to_send,"20670280");
    	fprintf(stderr,"%X,%X#\n",(unsigned int)ra_to_send,map_addr->dec);
		sprintf(t_tele_p,"%X,%X#",(unsigned int)ra_to_send,map_addr->dec);
		}
		sleep(1);
	//	sendbytes(t_tele_p, strlen(t_tele_p));
		globalCounter = globalCounter + 1;

		 if (globalCounter ==  REVOL_STEPS_COUNT)
			{
				globalCounter=0;
				sleep(1);
			}

}
void rotaryDeal_dec(void)
{

	char  telsscope_telegram [18]; /*8char;8char#*/
//	char* t_tele_p;
	float dec_to_send=0;

	//t_tele_p=telsscope_telegram;



		dec_to_send=((float)globalCounter/(float)REVOL_STEPS_COUNT)*MAX_NUMBER;
		map_addr->dec= (unsigned int)dec_to_send;


}
