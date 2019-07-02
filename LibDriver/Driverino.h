//
// Created by oscar on 13/06/19.
//

#ifndef RESHEX_DRIVERINO_H
#define RESHEX_DRIVERINO_H

// Setup driverino. Give sampling time and counts per revolution
void setupDriverino(uint16_t sampling_ms, int32_t cpr);

// Set Derivative constant
void setKd(float value);
// Set Integral constant
void setKi(float value);
// Set Proportional constant
void setKp(float value);

// Set saturation of duty cycle for motor pwm. From 0 to 255
void setSat(uint8_t value);
// Set death zone for motor pwm. From 0 to 255
void setDeath(uint8_t value);
// Set limit for motor pwm change per pid call (each sampling time). From 0 to 255
void setRateLimit(uint8_t value);

// Set reference for a motor in DEGREES
void setRef(uint8_t motor, int32_t pos);

// Return the angle recorded by the encoder in DEGREES
float getPos(uint8_t motor);
#endif //RESHEX_DRIVERINO_H
