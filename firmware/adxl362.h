/*
AVR library for the Analog Devices ADXL362 
ultra low power 3 axis accelerometer
Written for attiny device's USI

Matt Ruffner 2014
 */

#ifndef ADXL362_H
#define ADXL362_H

#include <avr/io.h>
#include <inttypes.h>


//////////////////////
// funtion definitions
//////////////////////

// preliminary
void adxl362_begin();

// measurement
void read_all_axes(int16_t * data);
void read_all_axes_short(int8_t * data);
int16_t read_axis(int axis);
int8_t read_axis_short(int axis);
int16_t read_temp();

// configuration
void config_linkloop(uint8_t mode);
void config_measure_mode(uint8_t mode); // requried call after reset
void config_range(uint8_t range);
void config_odr(uint8_t odr);
void config_int1(uint8_t int_source, uint8_t direction, uint8_t pullup);
void config_int2(uint8_t int_source, uint8_t direction, uint8_t pullup);
void config_act_int(uint16_t thresh, uint8_t time);
void config_inact_int(uint16_t thresh, uint16_t time);

// basic io
uint8_t read_reg(uint8_t reg);
uint16_t read_reg16(uint8_t reg);
void read_fifo(uint8_t * data, uint16_t len);
void write_reg(uint8_t reg, uint8_t value);
void write_reg16(uint8_t reg, uint16_t value);
uint8_t usi_transfer(uint8_t data);


// USI pin definitions
#define ADXL_DDR     DDRA
#define ADXL_PORT    PORTA
#define ADXL_MOSI    PA5
#define ADXL_MISO    PA6
#define ADXL_SCK     PA4
#define ADXL_CS      PA3
#define ADXL_CS_HIGH ADXL_PORT |= (1 << ADXL_CS)
#define ADXL_CS_LOW  ADXL_PORT &= ~(1 << ADXL_CS)

// interrupt pin definitions if ya want em
#define ADXL362_INT1_DDR  DDRB
#define ADXL362_INT1_PORT PORTB
#define ADXL362_INT1_PIN  PINB
#define ADXL362_INT2_DDR  DDRB
#define ADXL362_INT2_PORT PORTB
#define ADXL362_INT2_PIN  PINB
#define ADXL362_INT1      PB2
#define ADXL362_INT2      PB1

// axis definitions for read functions
#define X 0
#define Y 1
#define Z 2

///////////////////////////////
// ADXL362 register definitions
///////////////////////////////

// spi commands
#define ADXL362_WRITE_REGISTER 0x0A
#define ADXL362_READ_REGISTER  0x0B
#define ADXL362_READ_FIFO      0x0D

// identification registers, will always read the same
#define DEVID_AD    0x00
#define DEVID_MST   0x01
#define PARTID      0x02
#define REVID       0x03

// 8 bit resolution, power saving, registers
#define XDATA       0x08
#define YDATA       0x09
#define ZDATA       0x0A

// 12 bit, full resolution, sign extended data registers
// therefor the 4 MSBs of the _H register are all the same 
// and contain the sign of the reading
#define XDATA_L  0x0E
#define XDATA_H  0x0F
#define YDATA_L  0x10
#define YDATA_H  0x11
#define ZDATA_L  0x12
#define ZDATA_H  0x13

// 12 bit sign extended temperature registers
#define TEMP_L 0x14
#define TEMP_H 0x15

// status register
#define ADXL_STATUS          0x0B
#define ERR_USER_REGS   7
#define AWAKE           6
#define INACT           5
#define ACT             4
#define FIFO_OVERRUN    3
#define FIFO_WATERMARK  2
#define FIFO_READY      1
#define DATA_READY      0

// soft reset register, write 0x52, 'R' in ascii, to reset the devce
#define SOFT_RESET 0x1F

// FIFO entries register, the 10 bit number representing the
//  how many valid data samples are int the FIFO buffer
#define FIFO_ENTRIES_L  0x0C
#define FIFO_ENTRIES_H  0x0D

// activity threshold registers
// 11 bit, unsigned value to be compared with the 12 bit
// signed acceleration value
#define THRESH_ACT_L 0x20
#define THRESH_ACT_H 0x21

// activity time register, when used, the duration of motion 
// must be sustained for this amount of time to trigger activity
// detection caclulated by actual time = TIME_ACT/ODR
#define TIME_ACT 0x22

// inactivity threshold registers
// 11 bit unsigned value to be compared to the magnitude of the 
// acceleration value
#define THRESH_INACT_L 0x23
#define THRESH_INACT_H 0x24

// inactivity time registers, a 16 bit value
// actual time = TIME_INACT/ODR, a maximum of almost 90 minutes
#define TIME_INACT_L 0x25
#define TIME_INACT_H 0x26

// (in)activity control register
#define ACT_INACT_CTL 0x27
#define LINKLOOP   4 // 2 bit value
#define INACT_REF  3
#define INACT_EN   2
#define ACT_REF    1
#define ACT_EN     0

#define LINKLOOP_DEFAULT 0
#define LINKLOOP_LINKED  1
#define LINKLOOP_LOOP    2

// FIFO control register
#define FIFO_CONTROL 0x28
#define AH        3
#define FIFO_TEMP 2
#define FIFO_MODE 0 // 2 bit value

// FIFO samples register
// MSbit is AH in FIFO_CONTROL
#define FIFO_SAMPLES 0x29 // defaults to 0x80

// INT1/INT2 function map registers
#define INTMAP1 0x2A
#define INTMAP2 0x2B
#define INT_LOW        7
#define AWAKE          6 
#define INACT          5 
#define ACT            4
#define FIFO_OVERRUN   3
#define FIFO_WATERMARK 2
#define FIFO_READY     1
#define DATA_READY     0

// filter control register
#define FILTER_CTL 0x2C
#define RANGE      6    // 2 bit value
#define HALF_BW    4
#define EXT_SAMPLE 3
#define ODR        0    // 3 bit value

#define RANGE_2G 0
#define RANGE_4G 1
#define RANGE_8G 2

#define ODR_12_5_HZ 0
#define ODR_25_HZ   1
#define ODR_50_HZ   2
#define ODR_100_HZ  3 // reset default
#define ODR_200_HZ  4
#define ODR_400_HZ  5

// power control register
#define POWER_CTL 0x2D
#define EXT_CLK   6
#define LOW_NOISE 4     // 2 bit value
#define WAKEUP    3
#define AUTOSLEEP 2
#define MEASURE   0     // 2 bit value

#define MODE_STANDBY 0 // reset default
#define MODE_MEASURE 2

// self test register
#define SELF_TEST 0x2E
#define ST 0

#endif
