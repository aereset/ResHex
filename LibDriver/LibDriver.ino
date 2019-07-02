#include "Driverino.h"

#define SAMPLING_MS 10
#define CPR 3600
#define SATURATION 255
#define SLEW_RATE 5
#define DEATH 15
#define KI 0
#define KP 0.2

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);
  
  setupDriverino(SAMPLING_MS, CPR);
  setSat(SATURATION);
  setDeath(DEATH);
  setRateLimit(SLEW_RATE);
  setKp(KP); setKi(KI);

}

int pos = 0;
void loop() {
    delay(10);
}

void serialEvent() {
  pos = Serial.parseInt();
  setRef(0, pos);
}
