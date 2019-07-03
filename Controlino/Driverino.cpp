//
// Created by oscar on 13/06/19.
//
#include<stdint.h>
#include<Arduino.h>
#include <Wire.h>

#define ADDRESS 8

enum cmd_t {NONE, KP, KI, KD, SAT, DEATH, RATE, SETREF, GETPOS, CPR, SAMPLING};
typedef union driverinoUnion {
  struct driverinoStruct {
    uint8_t motor;
    enum cmd_t command;
    float value;
    float answer;
  } data; 
  uint8_t bin[11];
} DriverinoMsg;

void requestMsg(DriverinoMsg * msg, uint8_t address) {
  int i = 0;
  
  Wire.beginTransmission(address);
  for ( i = 0; i < 7; i++) {
      Wire.write(msg->bin[i]);
  }
  Wire.endTransmission();
  
  Wire.requestFrom(address, 4);    // request 4 bytes from slave device #8
  while (Wire.available()) { // slave may send less than requested
    msg->bin[i] = Wire.read(); // receive a byte as character
    i++;
  }
}


void setRef(uint8_t motor, int32_t pos) {
    DriverinoMsg msg;
    msg.data.motor = motor;
    msg.data.command = SETREF;
    msg.data.value = pos;
    requestMsg(&msg,ADDRESS);
}
void setKp(float value) {
    DriverinoMsg msg;
    msg.data.command = KP;
    msg.data.value = value;
    requestMsg(&msg,ADDRESS);
}
void setKi(float value) {
    DriverinoMsg msg;
    msg.data.command = KI;
    msg.data.value = value;
    requestMsg(&msg,ADDRESS);
}
void setKd(float value) {
    DriverinoMsg msg;
    msg.data.command = KD;
    msg.data.value = value;
    requestMsg(&msg,ADDRESS);
}
void setSat(uint8_t value) {
    DriverinoMsg msg;
    msg.data.command = SAT;
    msg.data.value = value;
    requestMsg(&msg,ADDRESS);
}
void setDeath(uint8_t value) {
    DriverinoMsg msg;
    msg.data.command = DEATH;
    msg.data.value = value;
    requestMsg(&msg,ADDRESS);
}
void setRateLimit(uint8_t value) {
    DriverinoMsg msg;
    msg.data.command = RATE;
    msg.data.value = value;
    requestMsg(&msg,ADDRESS);
}

void setSampling(uint16_t sampling_ms) {
    DriverinoMsg msg;
    msg.data.command = SAMPLING;
    msg.data.value = sampling_ms;
    requestMsg(&msg,ADDRESS);
}
void setCPR(int32_t cpr) {
    DriverinoMsg msg;
    msg.data.command = CPR;
    msg.data.value = cpr;
    requestMsg(&msg,ADDRESS);
}

void setupDriverino() {
    Wire.begin();
}
float getPos(uint8_t motor) {
    DriverinoMsg msg;
    msg.data.motor = motor;
    msg.data.command = GETPOS;
    requestMsg(&msg,ADDRESS);
    return msg.data.answer;
}
