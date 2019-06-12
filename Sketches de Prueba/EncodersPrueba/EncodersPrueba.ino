#define mbi(pos) (1 << (pos)) 
#define rbi(reg,bit) ((reg >> bit) & 0x01)    // Leer bit de reg 
#define sbi(reg,bit) (reg |= mbi(bit))      // Poner a 1 bit de reg 
#define cbi(reg,bit) (reg &= ~mbi(bit))     // Poner a 0 bit de reg 
#define tbi(reg,bit) (reg ^= mbi(bit))      // Cambiar de valor bit de reg 

volatile uint8_t lastPC1 = 0, nowPC1 = 0;
volatile uint8_t lastPC2 = 0, nowPC2 = 0;
volatile uint8_t aux = 0;
volatile uint8_t i = 0;

volatile long pulses[6] = {0,0,0,0,0,0};

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
void setup() {
  // Pin change interrupts of PCI1
  sbi(PCICR,PCIE1);    // Enable PCINT15:8
  sbi(PCMSK1,PCINT9);  // Enable PCINT mask on ENCA0
  sbi(PCMSK1,PCINT10); // Enable PCINT mask on ENCA1
  sbi(PCMSK1,PCINT11); // Enable PCINT mask on ENCA2
  sbi(PCMSK1,PCINT12); // Enable PCINT mask on ENCA3
  sbi(PCMSK1,PCINT13); // Enable PCINT mask on ENCA4
  sbi(PCMSK1,PCINT14); // Enable PCINT mask on ENCA5

  // Pin change interrupts of PCI2
  sbi(PCICR,PCIE2);    // Enable PCINT23:16
  sbi(PCMSK2,PCINT16); // Enable PCINT mask on ENCB0
  sbi(PCMSK2,PCINT17); // Enable PCINT mask on ENCB1
  sbi(PCMSK2,PCINT18); // Enable PCINT mask on ENCB2
  sbi(PCMSK2,PCINT19); // Enable PCINT mask on ENCB3
  sbi(PCMSK2,PCINT20); // Enable PCINT mask on ENCB4
  sbi(PCMSK2,PCINT21); // Enable PCINT mask on ENCB5

  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 6; i++) {
      Serial.print(pulses[i]); Serial.print(" ");
  }
  Serial.println();
  delay(100);
}
