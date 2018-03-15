#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

#include "gpiolib.h"

/* globale Variable - zum Beenden der Schleife in main */
int loop = 1;

/* Signalhandler fuer STRG-C */
void  finish(int sig)
  {
  printf("Signal %d empfangen. Programm wird beendet.\n", sig);
  loop = 0;
  }

/* ZUM TESTEN: Flanke an GPIO 17 = Pin 11  erkennen */
int main(void)
  {
  int ret;
  struct sigaction sig_struct;

  /* Signalhandler fuer STRG-C einrichten */
  sig_struct.sa_handler = finish;
  sigemptyset(&sig_struct.sa_mask);
  sig_struct.sa_flags = 0;
  sigaction(SIGINT,&sig_struct,NULL);

  /* GPIO konfigurieren */
  gpio_export(17);
  gpio_direction(17, IN);
  gpio_edge(17, 'f');

  /* Flankenerkennung */
  while(loop)
    {
    ret = gpio_wait(17, 500);
    if(ret < 0)
      printf("*** Error poll(): %d\n", ret);
    else if(ret == 0)
      printf("*** Timeout\n");
    else
      printf("*** Edge detected: %d\n", ret);
    }
  gpio_unexport(27);
  return 0;
  }
