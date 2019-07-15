#include "Driverino.h"
#include <Console.h>

#define SAMPLING_MS 10
#define COUNTS 3600
#define SATURATION 255
#define SLEW_RATE 5
#define Death 15
#define Ki 0
#define Kp 0.2

void setup() {
  Serial.begin(1000000);

  setupDriverino();

  delay(1000);
  
  setRef(0, 0);   
  setRef(1, 0);  
  setRef(2, 0); 
  setRef(3, 0);  
  setRef(4, 0);
  setRef(5, 0); 
  

}

long int last = 0;
void loop() {
  if (Console.available() > 0) {
    int poss = Console.parseInt();
    Console.println(poss);
    setRef(0, poss);
  }

  if (millis() - last > 1000) {
    last = millis();
    for (int i = 0; i < 6; i++) {
      Serial.print(getPos(i)); Serial.print("\t"); delay(10);
    }
    Serial.println();
  }

}
