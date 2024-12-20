#include "i2c_comm.h"
#define mpu6050_address 0b1101000

//1. function that will read sensor's signed 16 bit data in x, y, z direction
int16_t read_mpu(uint8_t addy){
  //begin transmission from mpu6050 to arduino
  Wire.beginTransmission(mpu6050_address);
  //tell mpu6050 to read the address specified in function call
  Wire.write(addy);
  //send a repeated start condition to keep transmission open
  Wire.endTransmission(false);
  //arduino request 16 bits or 2 bytes from mpu6050 
  Wire.requestFrom(mpu6050_address, 2);
  //check if enough data was received by arduino (master)
    //if less than 2 bytes were received, error code and return 0
  if (Wire.available() < 2) {
    Serial.println("not enough bytes received, check scl/sda connection!!");
    return 0;
  }
  //store the first read byte which represents data from 15-8
  int8_t msbyte = Wire.read();
  //store the second byte which represents data from 7-0
  int8_t lsbyte = Wire.read();
  //combine stored bytes to create signed 16 bit number
  return (msbyte << 8) | lsbyte;
}



//2. funcion that will write to sensor's register to initialize i2c protocol
void write_mpu(uint8_t addy, uint8_t data){
  //begin i2C tranmission from mpu6050 to arduino
  Wire.beginTransmission(mpu6050_address);
  //specify register address to write to
  Wire.write(addy); 
  //write the data value to said register
  Wire.write(data);
  //if the tranmission fails, print error message
  if (Wire.endTransmission() != 0){
    Serial.println("error writing to mpu6050!");
    Serial.println(addy, HEX);
  }
} 