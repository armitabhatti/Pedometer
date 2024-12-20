#ifndef WEB_H
#define WEB_H

#include <WiFi.h>
#include "Arduino.h"

void wifi_init ();
void website (int16_t accelX, int16_t accelY, int16_t accelZ);
double accel_mag (int16_t accelX, int16_t accelY, int16_t accelZ);
int detect_steps(int16_t accelX, int16_t accelY, int16_t accelZ);


#endif