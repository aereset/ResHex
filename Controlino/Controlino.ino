#include "Driverino.h"

#define SAMPLING_MS 10
#define COUNTS 3600
#define SATURATION 255
#define SLEW_RATE 5
#define DEATH 15
#define KI 0
#define KP 0.2
 
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(10);               

  setupDriverino();
  setSampling(SAMPLING_MS);
  setCPR(COUNTS);
  setSat(SATURATION);
  setDeath(DEATH);
  setRateLimit(SLEW_RATE);
  setKp(KP); setKi(KI);
   
}

 
int pos = 0;
void loop() {
  Serial.println(getPos(0));
    delay(100);
}
 
void serialEvent() {
  pos = Serial.parseInt();
  setRef(0, pos);
}
 
