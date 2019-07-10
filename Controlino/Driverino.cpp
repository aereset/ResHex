//
// Created by oscar on 13/06/19.
//
#include "Driverino.h"
#include<stdint.h>
#include<Arduino.h>
#include <SoftwareSerial.h>
#include <Console.h>
#define TERMINATOR 0x02

#define DEBUG
 SoftwareSerial softSerial(8, 9);

uint16_t ChecksumFletcher16(byte *data, size_t count )
{
  uint8_t sum1 = 0;
  uint8_t sum2 = 0;

  for (size_t index = 0; index < count; ++index )
  {
    sum1 = sum1 + (uint8_t)data[index];
    sum2 = sum2 + sum1;
  }
  return (sum2 << 8) | sum1;
}
void requestMsg(MasterMsg * masterMsg, SlaveMsg * slaveMsg) {
  
  masterMsg->data.motor = 0;
  masterMsg->data.command = DEATH;
  masterMsg->data.value = millis() / 1000.0;
  masterMsg->data.checksum = ChecksumFletcher16(masterMsg->bin, 7);
  masterMsg->data.terminator = TERMINATOR;

  softSerial.write(masterMsg->bin, 10);
  delayMicroseconds(1000);

  softSerial.readBytesUntil(TERMINATOR, slaveMsg->bin, 7);
  if (slaveMsg->data.checksum == ChecksumFletcher16(slaveMsg->bin, 4)) {
    Console.println("[MASTER] driverinoSrv:");
    Console.println(slaveMsg->data.answer);
  } else {
    Console.println("ERROR");
  }


}


void setRef(uint8_t motor, int32_t pos) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.motor = motor;
    msg.data.command = SETREF;
    msg.data.value = pos;
    requestMsg(&msg,&slaveMsg);
}
void setKp(float value) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = KP;
    msg.data.value = value;
    requestMsg(&msg,&slaveMsg);
}
void setKi(float value) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = KI;
    msg.data.value = value;
    requestMsg(&msg,&slaveMsg);
}
void setKd(float value) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = KD;
    msg.data.value = value;
    requestMsg(&msg,&slaveMsg);
}
void setSat(uint8_t value) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = SAT;
    msg.data.value = value;
    requestMsg(&msg,&slaveMsg);
}
void setDeath(uint8_t value) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = DEATH;
    msg.data.value = value;
    requestMsg(&msg,&slaveMsg);
}
void setRateLimit(uint8_t value) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = RATE;
    msg.data.value = value;
    requestMsg(&msg,&slaveMsg);
}

void setSampling(uint16_t sampling_ms) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = SAMPLING;
    msg.data.value = sampling_ms;
    requestMsg(&msg,&slaveMsg);
}
void setCPR(int32_t cpr) {
    MasterMsg msg;
    SlaveMsg slaveMsg;
    msg.data.command = CPR;
    msg.data.value = cpr;
    requestMsg(&msg,&slaveMsg);
}

void setupDriverino() {

  Bridge.begin();
  Console.begin();

  softSerial.begin(38400);
}
float getPos(uint8_t motor) {
    MasterMsg masterMsg;
    SlaveMsg slaveMsg;
    masterMsg.data.motor = motor;
    masterMsg.data.command = GETPOS;
    requestMsg(&masterMsg,&slaveMsg);
    
    return slaveMsg.data.answer;
}
