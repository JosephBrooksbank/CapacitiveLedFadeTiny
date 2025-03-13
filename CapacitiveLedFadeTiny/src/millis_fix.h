#include "Arduino.h"

volatile unsigned long timer_millis = 0;

void update_millis()
{
  static unsigned long last_micros = 0;
  unsigned long current_micros = micros();
  if (current_micros - last_micros >= 1000)
  {
    timer_millis++;
    last_micros = current_micros;
  }
}