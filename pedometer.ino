//MPU6050 i2c communication using wire.h library with arduino uno
#include "Wire.h"

//wifi for esp32
#include <WiFi.h>

#include "i2c_comm.h"
#include "web.h"

/*--------------------------------------------------------------- Define Addresses ---------------------------------------------- */
//from the ivensense mpu6050 product specifications, i2c address is 1101000

#define mpu6050_address 0b1101000

//from the ivensense mpu6050 Register Map and Descriptions

//1. power management allows mpu6050 to be woken up from sleep more
  //set bit 6 to 0 to disable sleep mode, 
#define pwr_mgmt_1 0x6b

//2. accelerometer configuration allows us to use the full range of the accelerometer
 //for the application of a pedometer, more accuracy is desired, so set to +-4g by setting bits 3 and 4 to 01
 #define accel_config 0x1c

//3. gyroscope configuration allows us to use full scale range of the gyroscope
  //for the application of a pedometer, more accuracy is desired, so set to +-250 by setting bits 3 and 4 to 0
#define gyro_config 0x1b

//4. accelerometer data is stored in registers 59-64 with the most recent measurements
 //each x, y, z acceleromter measurement is a 16 bit 2's complement 
#define accel_xout 0x3b 
#define accel_yout 0x3d
#define accel_zout 0x3f

//5. gyroscope data is stored in registers 67-72 with the most ecent measurements
  //each x, y, z gyroscope measurement is a 16 buit 2's complement
#define gyro_xout 0x43
#define gyro_yout 0x45
#define gyro_zout 0x47

// connect designated scl and sda pins to arduino uno
#define scl 22
#define sda 21

/*--------------------------------------------------------- Initialization --------------------------------------------------------*/

void setup(){

  //start the serial communication, where values will be displayed
  Serial.begin(115200); 
  //initialize the arduino as the master device by calling begin function with no argument
  Wire.begin(sda, scl);
  //wake up mpu6050 by setting 6th bit to 0 (0x40 --> 0x00)
  while (read_mpu (pwr_mgmt_1) != 0x00){
    write_mpu (pwr_mgmt_1, 0x00);
    Serial.println("waking up....");
  }
   
  //configure mpu6050 acceleromater by setting 3rd and 4th bit to 01 --> 0x08
  write_mpu (accel_config, 0x08);
  // + gyroscope by setting 3rd and 4th bit to 0 (0x00 --> 0x00)
  write_mpu (gyro_config, 0x00);
  Serial.println("MPU6050 has been initialized!!");
  //Connect to wifi and initialize server
  wifi_init();
}

/*--------------------------------------------------------- Read and Print Data --------------------------------------------------------*/

void loop() {

  //read accelerometer data
  int16_t accelX = read_mpu(accel_xout);
  int16_t accelY = read_mpu(accel_yout);
  int16_t accelZ = read_mpu(accel_zout);

  double accelmag = accel_mag(accelX, accelY, accelZ);
  int steps = detect_steps(accelX, accelY, accelZ);

  Serial.print(accelX);
  Serial.print(",");
  Serial.print(accelY);
  Serial.print(",");
  Serial.print(accelZ);
  Serial.print(",");
  Serial.print(accelmag);
  Serial.print(",");
  Serial.println(steps);

  delay(1000);  // Wait 1 second

  website(accelX, accelY, accelZ);


}
 






