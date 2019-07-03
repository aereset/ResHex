#include "Driverino.h"

#include <Wire.h>
#define DRIVERINO_ADDR 8
#define SAMPLING_MS 10
#define COUNTS 3600

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

DriverinoMsg msg;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  Wire.begin(DRIVERINO_ADDR);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  
  setupDriverino(SAMPLING_MS, COUNTS);
}
void receiveEvent(int howMany) {
  int i = 0;
  while (1 < Wire.available()) {
    msg.bin[i] = Wire.read(); 
    i++;
  }
  processMsg();
}
void requestEvent() {
  for (int i = 7; i < 11; i++) {
      Wire.write(msg.bin[i]);
  }
}
void processMsg() {
  switch (msg.data.command) {
    case NONE:
    msg.data.answer = NAN;
    break;
    
    case KP:
    msg.data.answer = 1;
    setKp(msg.data.value);
    break;

    case KI:
    msg.data.answer = 1;
    setKi(msg.data.value);
    break;

    case KD:
    msg.data.answer = 1;
    setKd(msg.data.value);
    break;

    case SAT:
    msg.data.answer = 1;
    setSat(msg.data.value);
    break;

    case DEATH:
    msg.data.answer = 1;
    setDeath(msg.data.value);
    break;

    case RATE:
    msg.data.answer = 1;
    setRateLimit(msg.data.value);
    break;

    case SETREF:
    if (msg.data.motor < 6) {
      msg.data.answer = 1;
      setRef(msg.data.motor,msg.data.value);
    } else
      msg.data.answer = NAN;
    break;

    case GETPOS:
    if (msg.data.motor < 6) {
      msg.data.answer = getPos(msg.data.motor);
      setRef(msg.data.motor,msg.data.value);
    } else
      msg.data.answer = NAN;
    break;

    case CPR:
    msg.data.answer = 1;
    setCPR(msg.data.value);
    break;

    case SAMPLING:
    msg.data.answer = 1;
    setSampling(msg.data.value);
    break;
  }
}

void loop() {
    delay(10);
}
