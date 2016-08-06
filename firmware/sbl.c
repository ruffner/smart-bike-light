#include <util/delay.h>
#include <avr/io.h> 
#include <inttypes.h>
#include <avr/interrupt.h>

#include "adxl362.h"

void init_accel();
void init_ints();

#define TEST_LED_ON  PORTA |= (1 << PA7)
#define TEST_LED_OFF PORTA &= ~(1 << PA7)

int main()
{
  // set up motion interrupts
  init_accel();

  // set up our interrupts
  //  init_ints();

  DDRA |= (1 << PA7); // config test led as output
  TEST_LED_ON;
  _delay_ms(300);
  TEST_LED_OFF;
  _delay_ms(200);
  TEST_LED_ON;
  _delay_ms(300);
  TEST_LED_OFF;

  //sei();

  while ( 42 ){
    _delay_ms(1000);
    int8_t x_val = read_axis(X);

    
    for ( ;x_val > 0; x_val-- ){
      PORTA ^= (1 << PA7);
      _delay_ms(200);

      PORTA ^= (1 << PA7);
      _delay_ms(200);

    }

  }
  
  return 0;
}

ISR( EXT_INT0_vect )
{
  TEST_LED_ON;
}

ISR( PCINT1_vect )
{
  TEST_LED_OFF;
}

void init_ints()
{
  // rising edge of INT0 pin, activity detection
  MCUCR |= (1 << ISC01) | (1 << ISC00);

  // INT0 and pin change interrupt enable
  GIMSK |= (1 << INT0) | (1 << PCIE1);

  PCMSK1 |= (1 << PCINT9);
}

void init_accel()
{
  
  adxl362_begin();
  config_measure_mode(MODE_MEASURE);

  
  /*config_odr(ODR_12_5_HZ);

  config_linkloop(LINKLOOP_LOOP);

  config_int1( 1 << ACT, 0, 0 );
  
  config_int2( 1 << INACT, 0, 0 );

  config_act_int(1000, 13); // roughly a seconds worth of 1g

  config_inact_int(300, 45); // roughly three seconds worth of stillness
  */
}
