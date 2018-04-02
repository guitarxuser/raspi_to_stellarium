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

static int update(long *ptr)
{
  return((*ptr)++);
    	/* return value before increment */
}



int main(void)
{
  int	fd,i=0,counter=0;
	pid_t	pid;
	void	*area;

	if ((fd = open("/dev/zero", O_RDWR)) < 0)
	  fprintf(stderr,"open error\n");
	if ((area = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED,
	  fd, 0)) == MAP_FAILED)
	  fprintf(stderr,"mmap error\n");
	close(fd);		/* can close /dev/zero now that it's mapped */
        
	if ((pid = fork()) < 0) {
	  fprintf(stderr,"fork error\n");
	} else if (pid > 0) {			/* parent */
	 
	  for(i=0;i<5;i++)
	    {
              fprintf(stderr,"I am parent\n");
	      //      counter=update((long*)area);
	      fprintf(stderr,"parents counter is %d\n", counter);
            }
	}
        else {						/* child */
                  fprintf(stderr,"I am child\n");
		  	  for(i=0;i<5;i++)
	     {
                  fprintf(stderr,"I am child\n");
		  counter=update((long*)area);
	          fprintf(stderr,"childs counter is %d\n", counter);		  
             }
 }

	exit(0);
}
