#ifndef _gpio_TOOLS_H
#define _gpio_TOOLS_H

/* Bibliothek fuer den GPIO-Zugriff über die Systemschnittstelle */
/* Compile with: gcc -Wall -o program program.c gpiolib.c */

/* Symbolische Namen fuer die Datenrichtung und die Daten */
#define IN  0
#define OUT 1

#define LOW  0
#define HIGH 1

/* Datenpuffer fuer die GPIO-Funktionen */
#define gpio_MAXBUF 100

int gpio_export(int pin);
  /* GPIO-Pin aktivieren
   * Schreiben Pinnummer pin nach /sys/class/gpio/export
   * Ergebnis: 0 = O.K., -1 = Fehler
   */

int gpio_unexport(int pin);
  /* GPIO-Pin deaktivieren
   * Schreiben Pinnummer pin nach /sys/class/gpio/unexport
   * Ergebnis: 0 = O.K., -1 = Fehler
   */

int gpio_direction(int pin, int dir);
  /* Datenrichtung des GPIO-Pins Nummer pin festlegen
   * dir = [IN|OUT]
   * Schreiben Pinnummer nach /sys/class/gpioXX/direction
   * Ergebnis: 0 = O.K., -1 = Fehler
   */

int gpio_read(int pin);
  /* vom GPIO-Pin Nummer pin lesen
   * Ergebnis: -1 = Fehler, 0/1 = Portstatus
   */

int gpio_write(int pin, int value);
  /* auf GPIO-Pin Nummer pin schreiben
   * value = [LOW|HIGH]
   * Ergebnis: -1 = Fehler, 0 = O.K.
   */

int gpio_edge(unsigned int pin, char edge);
  /* GPIO-Pin Nummer pin auf Detektion einer Flanke setzen.
   * Fuer die Flanke (edge) koennen folgende Parameter gesetzt werden:
   * 'r' (rising) - steigende Flanke,
   * 'f' (falling) - fallende Flanke,
   * 'b' (both) - beide Flanken,
   * 'n' (none) - keine Flankenerkennung.
   */

int gpio_wait(unsigned int pin, int timeout);
  /* Warten auf Flanke am GPIO-Pin.
   * Eingabewerte: pin: GPIO-Pin
   *               timeout: Wartezeit in Millisekunden
   * Der Pin muss voher eingerichtet werden (export,
   * direction, edge)
   * Rueckgabewerte: <0: Fehler, 0: poll() Timeout,
   * 1: Flanke erkannt, Pin lieferte "0"
   * 2: Flanke erkannt, Pin lieferte "1"
   */

int delay(unsigned long millis);
  /* Delay, Zeitangabe in Millisekunden
   */

#endif


