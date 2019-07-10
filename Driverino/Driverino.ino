#include "Driverino.h"
#include <Wire.h>

#define DEBUG
#define DRIVERINO_ADDR 8
#define SAMPLING_MS 10
#define COUNTS 3600
#define TERMINATOR 0x02

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


MasterMsg masterMsg;
SlaveMsg slaveMsg;

void setup() {
  Serial2.begin(38400);
  Serial.begin(9600);
  Serial.setTimeout(10);

  setupDriverino(SAMPLING_MS, COUNTS);
}

void serialEvent2() {
  Serial2.readBytesUntil(TERMINATOR, masterMsg.bin, 10);
  if (masterMsg.data.checksum == ChecksumFletcher16(masterMsg.bin, 7)) {
    processMsg();
    slaveMsg.data.checksum = ChecksumFletcher16(slaveMsg.bin, 4);
    slaveMsg.data.terminator = TERMINATOR;
    Serial2.write(slaveMsg.bin, 7);

    Serial.println("[SLAVE] driverinoSrv:");
    Serial.print("\tmotor: "); Serial.println(masterMsg.data.motor);
    Serial.print("\tcommand: "); Serial.println(masterMsg.data.command);
    Serial.print("\tvalue: "); Serial.println(masterMsg.data.value);
  } else {
    Serial.println("ERROR");

  }
}


void processMsg() {
  switch (masterMsg.data.command) {
    case NONE:
    slaveMsg.data.answer = NAN;
    break;
    
    case KP:
    slaveMsg.data.answer = 1;
    setKp(masterMsg.data.value);
    break;

    case KI:
    slaveMsg.data.answer = 1;
    setKi(masterMsg.data.value);
    break;

    case KD:
    slaveMsg.data.answer = 1;
    setKd(masterMsg.data.value);
    break;

    case SAT:
    slaveMsg.data.answer = 1;
    setSat(masterMsg.data.value);
    break;

    case DEATH:
    slaveMsg.data.answer = 1;
    setDeath(masterMsg.data.value);
    break;

    case RATE:
    slaveMsg.data.answer = 1;
    setRateLimit(masterMsg.data.value);
    break;

    case SETREF:
    if (masterMsg.data.motor < 6) {
      slaveMsg.data.answer = 1;
      setRef(masterMsg.data.motor,masterMsg.data.value);
    } else
      slaveMsg.data.answer = NAN;
    break;

    case GETPOS:
    if (masterMsg.data.motor < 6) {
      slaveMsg.data.answer = getPos(masterMsg.data.motor);
      setRef(masterMsg.data.motor,masterMsg.data.value);
    } else
      slaveMsg.data.answer = NAN;
    break;

    case CPR:
    slaveMsg.data.answer = 1;
    setCPR(masterMsg.data.value);
    break;

    case SAMPLING:
    slaveMsg.data.answer = 1;
    setSampling(masterMsg.data.value);
    break;
  }

}

void loop() {
    delay(10);
    Serial.println(getPos(0));
}


uint16_t ChecksumFletcher16(byte *data, size_t count ) {
  uint8_t sum1 = 0;
  uint8_t sum2 = 0;

  for (size_t index = 0; index < count; ++index )
  {
    sum1 = sum1 + (uint8_t)data[index];
    sum2 = sum2 + sum1;
  }
  return (sum2 << 8) | sum1;
}
