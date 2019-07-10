//
// Created by oscar on 13/06/19.
//

#ifndef RESHEX_DRIVERINO_H
#define RESHEX_DRIVERINO_H
#include <stdint.h>

enum cmd_t {NONE, KP, KI, KD, SAT, DEATH, RATE, SETREF, GETPOS, CPR, SAMPLING};

typedef union masterMessage {
  struct masterStruct {
    uint8_t motor;
    enum cmd_t command;
    float value;
    uint16_t checksum;
    uint8_t terminator;
  } data;
  uint8_t bin[10];
} MasterMsg;

typedef union slaveMessage {
  struct slaveStruct {
    float answer;
    uint16_t checksum;
    uint8_t terminator;
  } data;
  uint8_t bin[7];
} SlaveMsg;


// Setup driverino with I2C address
void setupDriverino();

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

// Set sampling time
void setSampling(uint16_t sampling_ms);

// Set counts per revolution
void setCPR(int32_t cpr);

#endif //RESHEX_DRIVERINO_H
