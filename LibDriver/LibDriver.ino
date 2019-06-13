#include "Driverino.h"

#define SAMPLING_MS 10
#define CPR 9600
#define CPR_ARY (const uint32_t[]) {CPR,CPR,CPR,CPR,CPR,CPR}
#define SATURATION 127
#define KI 0
#define KP 1

void setup() {
    setupDriverino(SAMPLING_MS,CPR_ARY);
    setSat(SATURATION);
    setKp(KP); setKi(KI);

    Serial.begin(9600);
}

int pos = 0;
void loop() {
    Serial.print(pos); Serial.print(" "); Serial.println(getPos(0));
    delay(10);
}

void serialEvent() {
    pos = Serial.parseInt();
    setRef(0,pos);
}
