#include "Driverino.h"

#define DEBUG
#define DRIVERINO_ADDR 8
#define SAMPLING_MS 10
#define COUNTS 3600
#define TERMINATOR 0x02
#define COUNTS 3600
#define SATURATION 255
#define SLEW_RATE 5
#define Death 15
#define Ki 0
#define Kp 0.2

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

void setup() {
  pinMode(MISO, OUTPUT);

  SPCR = (1 << SPE) | (1 << SPIE);
  sei();
  
  Serial.begin(9600);
  Serial.setTimeout(10);

  setupDriverino(SAMPLING_MS, COUNTS);

  setSat(SATURATION);
  setDeath(Death); 
  setRateLimit(SLEW_RATE); 
  setKp(Kp); 
  setKi(Ki); 
}

uint8_t i = 0;
uint8_t j = 1;

ISR(SPI_STC_vect) {
  master.bin[i] = SPDR;
  asm volatile("nop");

  SPDR = slave.bin[j];

  if (i == sizeof(struct msg_s) - 1) i = 0;
  else i++;
  if (j == sizeof(struct msg_s) - 1) j = 0;
  else j++;
}

void processMsg() {
  slave.data.cmd = master.data.cmd;
  slave.data.motor = slave.data.motor;
  
  slave.data.value = getPos(slave.data.motor);

  switch (master.data.cmd) {
    case NONE:
    break;
    
    case KP:
    setKp(master.data.value);
    break;

    case KI:
    setKi(master.data.value);
    break;

    case KD:
    setKd(master.data.value);
    break;

    case SAT:
    setSat(master.data.value);
    break;

    case DEATH:
    setDeath(master.data.value);
    break;

    case RATE:
    setRateLimit(master.data.value);
    break;

    case SETREF:
    if (master.data.motor < 6) {
      setRef(master.data.motor,master.data.value);
    } else
      slave.data.value = NAN;
    break;

    case GETPOS:
    if (master.data.motor >= 6)
      slave.data.value = NAN;
    break;

    case CPR:
    setCPR(master.data.value);
    break;

    case SAMPLING:
    setSampling(master.data.value);
    break;

    default:
    slave.data.value = NAN;
  }

}

void loop() {
    delay(500);
}
