#define mbi(pos) (1 << (pos)) 
#define rbi(reg,bit) ((reg >> bit) & 0x01)    // Leer bit de reg 
#define sbi(reg,bit) (reg |= mbi(bit))      // Poner a 1 bit de reg 
#define cbi(reg,bit) (reg &= ~mbi(bit))     // Poner a 0 bit de reg 
#define tbi(reg,bit) (reg ^= mbi(bit))      // Cambiar de valor bit de reg 

void setup() {
  cli();
  sbi(DDRA,PA2);
  sbi(DDRC,PC2);
  sbi(DDRH,PH5);

  cbi(TCCR4A,COM4C0);
  sbi(TCCR4A,COM4C1);
  
  sbi(TCCR4A,WGM40);
  cbi(TCCR4A,WGM41);
  cbi(TCCR4B,WGM42);
  cbi(TCCR4B,WGM43);
  
  sbi(TCCR4B,CS40);
  sbi(TCCR4B,CS41);
  cbi(TCCR4B,CS42);

  TCNT4 = 0;
  OCR4C = 0;
  
  sei();

  sbi(PORTA,PA2);
  cbi(PORTC,PC2);

  OCR4C = 50;
  delay(1000);
  
  OCR4C = 0;
  delay(1000);

  cbi(PORTA,PA2);
  sbi(PORTC,PC2);

  OCR4C = 50;
  delay(1000);
  

  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(A1)); delay(50);
}
