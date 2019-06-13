//
// Created by oscar on 13/06/19.
//

#ifndef RESHEX_DRIVERINO_H
#define RESHEX_DRIVERINO_H

void setupDriverino(uint16_t sampling_ms, const uint32_t cpr[]);
void setSat(uint8_t value);
void setKd(float value);
void setKi(float value);
void setKp(float value);
void setRef(uint8_t motor, int32_t pos);
float getPos(uint8_t motor);
float getSpeed(uint8_t motor);

#endif //RESHEX_DRIVERINO_H
