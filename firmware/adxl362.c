/*
AVR library for the Analog Devices ADXL362 
ultra low power 3 axis accelerometer
Written for attiny device's USI

Matt Ruffner 2014
 */

#include "adxl362.h"

// need to call before anything
void adxl362_begin() {
  ADXL_DDR |= (1 << ADXL_CS);
  ADXL_DDR |= (1 << ADXL_MOSI);
  ADXL_DDR |= (1 << ADXL_SCK);
  ADXL_DDR &= ~(1 << ADXL_MISO);
  ADXL_PORT |= (1 << ADXL_MISO); // pullup

  ADXL_CS_HIGH;

  write_reg(SOFT_RESET, 0x52); // soft reset
}

void config_linkloop(uint8_t mode) {
  uint8_t cur = read_reg(ACT_INACT_CTL);
  cur &= 0x0F;
  cur |= (mode << LINKLOOP);
  write_reg(ACT_INACT_CTL, cur);
}

void config_measure_mode(uint8_t mode) {
  uint8_t cur = read_reg(POWER_CTL);
  cur &= 0xFC; // clear current mode
  cur |= (mode << MEASURE);
  write_reg(POWER_CTL, cur);
}

void config_range(uint8_t range) {
  uint8_t cur = read_reg(FILTER_CTL);
  cur &= 0x3F; // clear current range setting
  cur |= (range << RANGE);
  write_reg(FILTER_CTL, cur);
}

void config_odr(uint8_t odr) {
  uint8_t cur = read_reg(FILTER_CTL);
  cur &= 0xF8; // clear current odr setting
  cur |= (odr << ODR);
  write_reg(FILTER_CTL, cur);
}

void config_int1(uint8_t int_source, uint8_t direction, uint8_t pullup) {
  if(direction) {
    ADXL362_INT1_DDR |= (1 << ADXL362_INT1);
    if(pullup)
      ADXL362_INT1_PORT |= (1 << ADXL362_INT1);
  } else {
    ADXL362_INT1_DDR &= ~(1 << ADXL362_INT1);
  }
  write_reg(INTMAP1, int_source);
}

void config_int2(uint8_t int_source, uint8_t direction, uint8_t pullup) {
  if(direction) {
    ADXL362_INT2_DDR |= (1 << ADXL362_INT2);
    if(pullup)
      ADXL362_INT2_PORT |= (1 << ADXL362_INT2);
  } else {
    ADXL362_INT2_DDR &= ~(1 << ADXL362_INT2);
  }
  write_reg(INTMAP2, int_source);
}

void config_act_int(uint16_t thresh, uint8_t time) {
  write_reg16(THRESH_ACT_L, thresh);
  write_reg(TIME_ACT, time);

  uint8_t cur = read_reg(ACT_INACT_CTL);
  cur |= (1 << ACT_EN) | (1 << ACT_REF);
  write_reg(ACT_INACT_CTL, cur);
}

void config_inact_int(uint16_t thresh, uint16_t time) {
  write_reg16(THRESH_INACT_L, thresh);
  write_reg16(TIME_INACT_L, time);

  uint8_t cur = read_reg(ACT_INACT_CTL);
  cur |= (1 << INACT_EN) | (1 << INACT_REF);
  write_reg(ACT_INACT_CTL, cur);
}

void read_all_axes_short(int8_t * data) {
  ADXL_CS_LOW;
  usi_transfer(ADXL362_READ_REGISTER);
  usi_transfer(XDATA);
  data[0] = usi_transfer(0x00);
  data[1] = usi_transfer(0x00);
  data[2] = usi_transfer(0x00);
  ADXL_CS_HIGH;
}

void read_all_axes(int16_t * data) {
  ADXL_CS_LOW;
  usi_transfer(ADXL362_READ_REGISTER);
  usi_transfer(XDATA_L);
  data[0] = usi_transfer(0x00);
  data[0] += (usi_transfer(0x00) << 8);
  data[1] = usi_transfer(0x00);
  data[1] += (usi_transfer(0x00) << 8);
  data[2] = usi_transfer(0x00);
  data[2] += (usi_transfer(0x00) << 8);
  ADXL_CS_HIGH;
}

int16_t read_axis(int axis) {
  switch (axis) {
  case X:
    return (int16_t)read_reg16(XDATA_L);
    break;
  case Y:
    return (int16_t)read_reg16(YDATA_L);
    break;
  case Z:
    return (int16_t)read_reg16(ZDATA_L);
    break;
  }

  return (int16_t)read_reg16(ZDATA_L);
}

int16_t read_temp() {
  return read_reg16(TEMP_L);
}

int8_t read_axis_short(int axis) {
  switch (axis) {
  case X:
    return (int8_t)read_reg(XDATA);
  case Y:
    return (int8_t)read_reg(YDATA);
  case Z:
    return (int8_t)read_reg(ZDATA);
  }
  return (int8_t)read_reg(XDATA);
}

uint8_t read_reg(uint8_t reg) {
  uint8_t resp;
  ADXL_CS_LOW;
  usi_transfer(ADXL362_READ_REGISTER);
  usi_transfer(reg);
  resp = usi_transfer(0x00);
  ADXL_CS_HIGH;
  return resp;
}

uint16_t read_reg16(uint8_t reg) {
  uint16_t resp;
  ADXL_CS_LOW;
  usi_transfer(ADXL362_READ_REGISTER);
  usi_transfer(reg);
  resp = usi_transfer(0x00);
  resp += (usi_transfer(0x00)  << 8);
  ADXL_CS_HIGH;
  return resp;
}

void read_fifo(uint8_t * out, uint16_t len) {
  uint16_t i;
  ADXL_CS_LOW;
  usi_transfer(ADXL362_READ_FIFO);
  for(i = 0; i < len; i++)
    out[i] = usi_transfer(0x00);
  ADXL_CS_HIGH;
}

void write_reg(uint8_t reg, uint8_t value) {
  ADXL_CS_LOW;
  usi_transfer(ADXL362_WRITE_REGISTER);
  usi_transfer(reg);
  usi_transfer(value);
  ADXL_CS_HIGH;
}

void write_reg16(uint8_t reg, uint16_t value) {
  ADXL_CS_LOW;
  usi_transfer(ADXL362_WRITE_REGISTER);
  usi_transfer(reg);
  usi_transfer((uint8_t)value);
  usi_transfer((uint8_t)(value >> 8));
  ADXL_CS_HIGH;
}

uint8_t usi_transfer(uint8_t data) {
  USIDR = data;
  USISR = _BV(USIOIF); // clear flag
 
  while ( (USISR & _BV(USIOIF)) == 0 ) {
    USICR = (1<<USIWM0)|(1<<USICS1)|(1<<USICLK)|(1<<USITC);
  }
  return USIDR;
}
