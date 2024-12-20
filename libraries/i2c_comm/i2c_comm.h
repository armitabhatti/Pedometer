#ifndef i2c_comm_h
#define i2c_comm_h

#include "Wire.h"
#include "Arduino.h"

int16_t read_mpu(uint8_t addy);
void write_mpu(uint8_t addy, uint8_t data);

#endif