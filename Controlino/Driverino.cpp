//
// Created by oscar on 13/06/19.
//
#include "Driverino.h"
#include<stdint.h>
#include<Arduino.h>
#include <Wire.h>

#define ADDRESS 8

#define DEBUG

void requestMsg(DriverinoMsg * msg, uint8_t address) {
    Wire.beginTransmission(8);
    Wire.write(msg->bin,7);
    Wire.endTransmission();
    Wire.requestFrom(8,4);
    for(int i = 7; i < 11; i++) {
        msg->bin[i] = Wire.read();
    }

#ifdef DEBUG
Serial.println("driverinoSrv:");
Serial.print("\tmotor: "); Serial.println(msg->data.motor);
Serial.print("\tcommand: "); Serial.println(msg->data.command);
Serial.print("\tvalue: "); Serial.println(msg->data.value);
Serial.println("\t-----------------");
Serial.print("\tanswer: "); Serial.println(msg->data.answer);
#endif

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
