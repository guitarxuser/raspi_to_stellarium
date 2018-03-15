/* Bibliothek fuer den GPIO-Zugriff über die Systemschnittstelle */
/* Compile with: gcc -Wall -o program program.c gpiolib.c */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <poll.h>

#include "gpiolib.h"

/* Delay, Zeitangabe in Millisekunden
 */
int delay(unsigned long millis)
  {
  struct timespec ts;
  int err;

  ts.tv_sec = millis / 1000;
  ts.tv_nsec = (millis % 1000) * 1000000L;
  err = nanosleep(&ts, (struct timespec *)NULL);
  return (err);
  }

/* GPIO-Pin aktivieren
 * Schreiben Pinnummer nach /sys/class/gpio/export
 * Ergebnis: 0 = O.K., -1 = Fehler
 */
int gpio_export(int pin)
  {
  char buffer[gpio_MAXBUF];  /* Output Buffer   */
  ssize_t bytes;             /* Datensatzlaenge */
  int fd;                    /* Filedescriptor  */
  int res;                   /* Ergebnis von write */


  fd = open("/sys/class/gpio/export", O_WRONLY);
  if (fd < 0)
    {
    perror("Kann nicht auf export schreiben!\n");
    return(-1);
    }
  bytes = snprintf(buffer, gpio_MAXBUF, "%d", pin);
  res = write(fd, buffer, bytes);
  if (res < 0)
    {
    perror("gpio_export: Kann Pin nicht aktivieren!\n");
    return(-1);
    }

  close(fd);
  delay(100);
  return(0);
  }

/* GPIO-Pin deaktivieren
 * Schreiben Pinnummer nach /sys/class/gpio/unexport
 * Ergebnis: 0 = O.K., -1 = Fehler
 */
int gpio_unexport(int pin)
  {
  char buffer[gpio_MAXBUF];  /* Output Buffer   */
  ssize_t bytes;             /* Datensatzlaenge */
  int fd;                    /* Filedescriptor  */
  int res;                   /* Ergebnis von write */


  fd = open("/sys/class/gpio/unexport", O_WRONLY);
  if (fd < 0)
    {
    perror("gpio_unexport: Kann nicht auf unexport schreiben!\n");
    return(-1);
    }
  bytes = snprintf(buffer, gpio_MAXBUF, "%d", pin);
  res = write(fd, buffer, bytes);
  if (res < 0)
    {
    perror("gpio_unexport: Kann Pin nicht deaktivieren!\n");
    return(-1);
    }

  close(fd);
  return(0);
  }

/* Datenrichtung GPIO-Pin festlegen
 * Schreiben Pinnummer nach /sys/class/gpioXX/direction
 * Ergebnis: 0 = O.K., -1 = Fehler
 */
int gpio_direction(int pin, int dir)
  {
  char path[gpio_MAXBUF];      /* Buffer fuer Pfad   */
  int fd;                    /* Filedescriptor  */
  int res;                   /* Ergebnis von write */

  snprintf(path, gpio_MAXBUF, "/sys/class/gpio/gpio%d/direction", pin);
  fd = open(path, O_WRONLY);
  if (fd < 0)
    {
    perror("gpio_direction: Kann Datenrichtung nicht setzen (open)!\n");
    return(-1);
    }
  switch (dir)
    {
    case IN : res = write(fd,"in",2); break;
    case OUT: res = write(fd,"out",3); break;
    }
  if (res < 0)
    {
    perror("gpio_direction: Kann Datenrichtung nicht setzen (write)!\n");
    return(-1);
    }
  close(fd);
  return(0);
  }

/* vom GPIO-Pin lesen
 * Ergebnis: -1 = Fehler, 0/1 = Portstatus
 */
int gpio_read(int pin)
  {
  char path[gpio_MAXBUF];         /* Buffer fuer Pfad     */
  int fd;                       /* Filedescriptor       */
  char result[gpio_MAXBUF] = {0}; /* Buffer fuer Ergebnis */

  snprintf(path, gpio_MAXBUF, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_RDONLY);
  if (fd < 0)
    {
    perror("gpio_read: Kann vom GPIO nicht lesen (open)!\n");
    return(-1);
    }
  if (read(fd, result, 3) < 0)
    {
    perror("gpio_read: Kann vom GPIO nicht lesen (read)!\n");
    return(-1);
    }
  close(fd);
  return(atoi(result));
  }

/* auf GPIO-Pin schreiben
 * Ergebnis: -1 = Fehler, 0 = O.K.
 */
int gpio_write(int pin, int value)
  {
  char path[gpio_MAXBUF];    /* Buffer fuer Pfad   */
  int fd;                    /* Filedescriptor     */
  int res;                   /* Ergebnis von write */

  snprintf(path, gpio_MAXBUF, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_WRONLY);
  if (fd < 0)
    {
    perror("gpio_write: Kann auf GPIO nicht schreiben (open)!\n");
    return(-1);
    }
  switch (value)
    {
    case LOW : res = write(fd,"0",1); break;
    case HIGH: res = write(fd,"1",1); break;
    }
  if (res < 0)
    {
    perror("gpio_write: Kann auf GPIO nicht schreiben (write)!\n");
    return(-1);
    }
  close(fd);
  return(0);
  }

/* GPIO-Pin auf Detektion einer Flanke setzen.
 * Fuer die Flanke (edge) koennen folgende Parameter gesetzt werden:
 * 'r' (rising) - steigende Flanke,
 * 'f' (falling) - fallende Flanke,
 * 'b' (both) - beide Flanken.
 */
int gpio_edge(unsigned int pin, char edge)
  {
  char path[gpio_MAXBUF];    /* Buffer fuer Pfad   */
  int fd;                    /* Filedescriptor     */

  snprintf(path, gpio_MAXBUF, "/sys/class/gpio/gpio%d/edge", pin);

  fd = open(path, O_WRONLY | O_NONBLOCK );
  if (fd < 0)
    {
    perror("gpio_edge: Kann auf GPIO nicht schreiben (open)!\n");
    return(-1);
    }

  switch (edge)
    {
    case 'r': strncpy(path,"rising",8); break;
    case 'f': strncpy(path,"falling",8); break;
    case 'b': strncpy(path,"both",8); break;
    case 'n': strncpy(path,"none",8); break;
    default: return(-2);
    }
  write(fd, path, strlen(path) + 1);
  close(fd);
  return 0;
  }

/* Warten auf Flanke am GPIO-Pin.
 * Eingabewerte: pin: GPIO-Pin
 *               timeout: Wartezeit in Millisekunden
 * Der Pin muss voher eingerichtet werden (export,
 * direction, edge)
 * Rueckgabewerte: <0: Fehler, 0: poll() Timeout,
 * 1: Flanke erkannt, Pin lieferte "0"
 * 2: Flanke erkannt, Pin lieferte "1"
 */
int gpio_wait(unsigned int pin, int timeout)
  {
  char path[gpio_MAXBUF];    /* Buffer fuer Pfad   */
  int fd;                    /* Filedescriptor     */
  struct pollfd polldat[1];  /* Variable fuer poll() */
  char buf[gpio_MAXBUF];     /* Lesepuffer */
  int rc;                    /* Hilfsvariablen */

  /* GPIO-Pin dauerhaft oeffnen */
  snprintf(path, gpio_MAXBUF, "/sys/class/gpio/gpio%d/value", pin);
  fd = open(path, O_RDONLY | O_NONBLOCK );
  if (fd < 0)
    {
    perror("gpio_wait: Kann von GPIO nicht lesen (open)!\n");
    return(-1);
    }
  /* poll() vorbereiten */
  memset((void*)buf, 0, sizeof(buf));
  memset((void*)polldat, 0, sizeof(polldat));
  polldat[0].fd = fd;
  polldat[0].events = POLLPRI;
  /* eventuell anstehende Interrupts loeschen */
  lseek(fd, 0, SEEK_SET);
  rc = read(fd, buf, gpio_MAXBUF - 1);

  rc = poll(polldat, 1, timeout);
  if (rc < 0)
    { /* poll() failed! */
    perror("gpio_wait: Poll-Aufruf ging schief!\n");
    close(fd);
    return(-1);
    }
  if (rc == 0)
    { /* poll() timeout! */
    close(fd);
    return(0);
    }
  if (polldat[0].revents & POLLPRI)
    {
    if (rc < 0)
      { /* read() failed! */
      perror("gpio_wait: Kann von GPIO nicht lesen (read)!\n");
      close(fd);
      return(-2);
      }
    /* printf("poll() GPIO %d interrupt occurred: %s\n", pin, buf); */
    close(fd);
    return(1 + atoi(buf));
    }
  close(fd);
  return(-1);
  }

