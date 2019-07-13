//
// Created by oscar on 13/06/19.
//
#include "Driverino.h"
#include <stdint.h>
#include <Arduino.h>
#include <Console.h>

#define SPI_SS 4
float pos[6];

enum cmd_t {NONE, KP, KI, KD, SAT, DEATH, RATE, SETREF, GETPOS, CPR, SAMPLING};

struct msg_s {
  enum cmd_t cmd;
  uint8_t motor;
  float value;
};

typedef union msg_u {
  uint8_t bin[sizeof(struct msg_s)];
  struct msg_s data;
} Msg;
Msg slave, master;

void call() {
  for (size_t i = 0; i < sizeof(struct msg_s); i++) {
    digitalWrite(SPI_SS,LOW);
    SPDR = master.bin[i];
    while(!(SPSR & (1 << SPIF))){}
    digitalWrite(SPI_SS,HIGH);
    slave.bin[i] = SPDR;
    asm volatile("nop");
  }

  if (slave.data.motor < 6 && slave.data.value==slave.data.value)
    pos[slave.data.motor] = slave.data.value;
}

void setRef(uint8_t motor, int32_t pos) {
    master.data.motor = motor;
    master.data.cmd = SETREF;
    master.data.value = pos;
    call();
}
void setKp(float value) {
    master.data.cmd = KP;
    master.data.value = value;
    call();
}
void setKi(float value) {
    master.data.cmd = KI;
    master.data.value = value;
    call();
}
void setKd(float value) {
    master.data.cmd = KD;
    master.data.value = value;
    call();
}
void setSat(uint8_t value) {
    master.data.cmd = SAT;
    master.data.value = value;
    call();
}
void setDeath(uint8_t value) {
    master.data.cmd = DEATH;
    master.data.value = value;
    call();
}
void setRateLimit(uint8_t value) {
    master.data.cmd = RATE;
    master.data.value = value;
    call();
}

void setSampling(uint16_t sampling_ms) {
    master.data.cmd = SAMPLING;
    master.data.value = sampling_ms;
    call();
}
void setCPR(int32_t cpr) {
    master.data.cmd = CPR;
    master.data.value = cpr;
    call();
}
float getPos(uint8_t motor) {
    master.data.motor = motor;
    master.data.cmd = GETPOS;
    call();
    return pos[motor];
}

void setupDriverino() {

  Bridge.begin();
  Console.begin();

  pinMode(MOSI,OUTPUT);
  pinMode(SCK,OUTPUT);
  pinMode(SPI_SS,OUTPUT);

  SPCR = (1<<SPE) | (1<<MSTR) | (1<<SPR0);

}
