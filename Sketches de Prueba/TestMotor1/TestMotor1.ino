#define mbi(pos) (1 << (pos)) 
#define rbi(reg,bit) ((reg >> bit) & 0x01)    // Leer bit de reg 
#define sbi(reg,bit) (reg |= mbi(bit))      // Poner a 1 bit de reg 
#define cbi(reg,bit) (reg &= ~mbi(bit))     // Poner a 0 bit de reg 
#define tbi(reg,bit) (reg ^= mbi(bit))      // Cambiar de valor bit de reg 

void setup() {
  sbi(DDRA,PA5);
  sbi(DDRC,PC5);

  sbi(PORTA,PA5);
  cbi(PORTC,PC5);

  analogWrite(44,50);
  delay(1000);
  
  analogWrite(44,0);
  delay(1000);

  cbi(PORTA,PA5);
  sbi(PORTC,PC5);

  analogWrite(44,50);
  delay(1000);
  

  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A1)); delay(50);
}
