#include "Driverino.h"
#include <Wire.h>

#define SAMPLING_MS 10
#define COUNTS 3600
#define SATURATION 255
#define SLEW_RATE 5
#define Death 15
#define Ki 0
#define Kp 0.2
 
void setup() {
  setupDriverino();

  Serial.begin(9600);
  Serial.setTimeout(10);

  delay(10);

  setSampling(SAMPLING_MS);  delay(10);

  setCPR(COUNTS); delay(10);
  setSat(SATURATION); delay(10);
  setDeath(Death); delay(10);
  setRateLimit(SLEW_RATE); delay(10);
  setKp(Kp);  delay(10);
  setKi(Ki);  delay(10);
   
}

 
int pos = 0;
void loop() {
  Serial.println(getPos(0));
    delay(1000);
}
 
void serialEvent() {
  pos = Serial.parseInt();
  setRef(0, pos);
}
 
