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

  setupDriverino();

  delay(10);

  setRef(1, 1000); delay(10);
  setRef(3, 1000); delay(10);
  setRef(4, 1000); delay(10);
  setRef(5, 1000); delay(10);

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
      Console.print(getPos(i)); Console.print("\t");
    }
    Console.println();
  }

}
