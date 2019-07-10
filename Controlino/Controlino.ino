#include "Driverino.h"
#include <SoftwareSerial.h>
#include <Console.h>

#define SAMPLING_MS 10
#define COUNTS 3600
#define SATURATION 255
#define SLEW_RATE 5
#define Death 15
#define Ki 0
#define Kp 0.2

void setup() {
  
  setupDriverino();

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
long int last = 0;
void loop() {
  if (Console.available() > 0) {
      int pos = Console.parseInt();
      Console.println(pos);
      setRef(0, pos);
  }

  if (millis()- last > 1000) {
    last = millis();
      getPos(0);
  }

}
 
 
