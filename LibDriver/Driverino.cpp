//
// Created by oscar on 13/06/19.
//
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <string.h>
#include <Arduino.h>
/* ################################################
 *                      ENCODERS
 * ################################################ */
#define SAMPLES 10
static volatile uint8_t lastPC1 = 0, nowPC1 = 0;
static volatile uint8_t lastPC2 = 0, nowPC2 = 0;
static volatile uint8_t aux = 0;
static volatile uint8_t i = 0;
static volatile int32_t pulses[6] = {0,0,0,0,0,0};
// Interruption routine of PCI1
ISR(PCINT1_vect) {
        nowPC1 = PINJ; // Read A Channel
        nowPC2 = PINK; // Read B Channel

        aux = nowPC1 ^ lastPC1; // Detect changes on channel A with XOR
        lastPC1 = nowPC1; // Save channel A

        for (uint8_t i = 0; aux; i++, aux >>= 1, nowPC1 >>=1, nowPC2 >>= 1) { // Look for changes on pin
            if (aux & 0x01) { // Detected change on pin i
                if ((nowPC1 & 0x01) == (nowPC2 & 0x01))
                    pulses[i]++; // Rising edge on A and high on channel B or Falling edge on A and low on channel B
                else
                    pulses[i]--; // Rising edge on A and low on channel B or Falling edge on A and high on channel B
            }
        }
}

// Interruption routine of PCI2
ISR(PCINT2_vect) {
        nowPC1 = PINJ; // Read A Channel
        nowPC2 = PINK; // Read B Channel

        aux = nowPC2 ^ lastPC2; // Detect changes on channel B with XOR
        lastPC2 = nowPC2; // Save channel A

        for (uint8_t i = 0; aux; i++, aux >>= 1, nowPC1 >>=1, nowPC2 >>= 1) { // Look for changes on pin
            if (aux & 0x01) { // Detected change on pin i
                if ((nowPC1 & 0x01) != (nowPC2 & 0x01))
                    pulses[i]++; // Rising edge on B and low on channel B or Falling edge on B and high on channel B
                else
                    pulses[i]--; // Rising edge on B and high on channel B or Falling edge on B and low on channel A
            }
        }
}

void setupEncoders() {
    // Activate pin change interrupts 1 and 2
    PCICR = (1<<PCIE1) | (1<<PCIE2);
    // Enable mask from PCINT9 to PCINT14 for Channel A
    PCMSK1 = (1<<PCINT9) | (1<<PCINT10) | (1<<PCINT11) | (1<< PCINT12) | (1<<PCINT13) | (1<<PCINT14);
    // Enable mask from PCINT16 to PCINT21 for Channel B
    PCMSK2 = (1<<PCINT16) | (1<<PCINT17) | (1<<PCINT18) | (1<< PCINT19) | (1<<PCINT20) | (1<<PCINT21);
}


/* ################################################
 *                         PWM
 * ################################################ */
void setupPWM() {
    // Set as output channel A
    DDRA |= (1<<PA0) | (1<<PA1) | (1<<PA2) | (1<<PA3) | (1<<PA4) | (1<<PA5);
    // Set as output channel C
    DDRC |= (1<<PC0) | (1<<PC1) | (1<<PC2) | (1<<PC3) | (1<<PC4) | (1<<PC5);
    // Set as output pwm of drivers 0,1,2
    DDRH |= (1<<PH3) | (1<<PH4) | (1<<PH5);
    // Set as output pwm of drivers 3,4,5
    DDRL |= (1<<PL3) | (1<<PL4) | (1<<PL5);

    // Timer 4 config for drivers 0,1,2
    TCCR4A = (1<<COM4A1) | (1<<COM4B1) | (1<<COM4C1) | (1<<WGM40);
    TCCR4B = (1<<CS40) | (1<<CS41);
    TCNT4 = 0;
    OCR4A = 0;
    OCR4B = 0;
    OCR4C = 0;

    // Timer 5 config for drivers 0,1,2
    TCCR5A = (1<<COM5A1) | (1<<COM5B1) | (1<<COM5C1) | (1<<WGM50);
    TCCR5B = (1<<CS50) | (1<<CS51);
    TCNT5 = 0;
    OCR5A = 0;
    OCR5B = 0;
    OCR5C = 0;
}

void setPWM(uint8_t motor, int16_t pwm) {
    // Select direction using sign of pwm
    if(pwm > 0) {
        PORTA |= (1<<motor);
        PORTC &= ~(1<<motor);
    } else {
        PORTC |= (1<<motor);
        PORTA &= ~(1<<motor);
    }

    // Select motor
    switch(motor) {
        case 0:
            OCR4A = abs(pwm);
            break;
        case 1:
            OCR4B = abs(pwm);
            break;
        case 2:
            OCR4C = abs(pwm);
            break;
        case 3:
            OCR5A = abs(pwm);
            break;
        case 4:
            OCR5B = abs(pwm);
            break;
        case 5:
            OCR5C = abs(pwm);
            break;
    }
}

/* ################################################
 *                      PID
 * ################################################ */
static int32_t CPR;
static uint16_t sampleTime;
static volatile uint32_t sample = 0;
static float Kp = 0,Ki = 0,Kd = 0,sat = 255, death = 0;
static float error[6], lastError[6], sumError[6], refPos[6], speed[6];
static int16_t action[6];
ISR(TIMER3_COMPA_vect) {
    for (int i = 0; i < 6; i++) {
        // Compute error
        error[i] = refPos[i] - pulses[i];

        // Compute PID action
        action[i] = Kp*error[i]+Ki*sumError[i]+Kd*(error[i]-lastError[i]);

        // Apply saturation
        action[i] = constrain(action[i] ,-sat,sat);

        // If pwm is inside the death zone change it to zero
        if (abs(action[i]) < death) 
            action[i] = 0;
      
        // Move the motor
        setPWM(i,action[i]);

        // Sum the error and save last error
        sumError[i] += error[i];
        lastError[i] = error[i];
    }
    sample++;
}
void setupPID(uint16_t sampling_ms, int32_t cpr) {
    // Configure timer to compute PID perodically
    // Prescaler a 1024, CTC mode
    TCCR3A = 0;
    TCCR3B = (1<<CS32) | (1<<WGM32);
    OCR3A = sampling_ms * 15.625;
    TIMSK3 = (1<<OCIE3A);

    // Save sample time and pulses per revolution
    sampleTime = sampling_ms;
    CPR = cpr;
}
void setRef(uint8_t motor, int32_t pos) {
    // Save reference in degrees
    refPos[motor] = pos*((float)CPR)/360.0;
}
void setKp(float value) {
    Kp = value;
}
void setKi(float value) {
    Ki = value;
}
void setKd(float value) {
    Kd = value;
}
void setSat(uint8_t value) {
    sat = value;
}
void setDeath(uint8_t value) {
    death = value;
}

void setupDriverino(uint16_t sampling_ms, int32_t cpr) {
    // Call setup subroutines
    setupEncoders();
    setupPWM();
    setupPID(sampling_ms,cpr);
}
float getPos(uint8_t motor) {
    // return position in degrees
    return pulses[motor] / (float)CPR * 360.0;
}
