#include <util/delay.h>
#include <avr/io.h> 
#include <inttypes.h>
#include <avr/interrupt.h>

#include "adxl362.h"

int main()
{
  adxl362_begin();
  config_measure_mode(MODE_MEASURE);

  return 0;
}
