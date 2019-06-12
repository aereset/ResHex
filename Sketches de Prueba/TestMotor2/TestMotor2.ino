#define mbi(pos) (1 << (pos)) 
#define rbi(reg,bit) ((reg >> bit) & 0x01)    // Leer bit de reg 
#define sbi(reg,bit) (reg |= mbi(bit))      // Poner a 1 bit de reg 
#define cbi(reg,bit) (reg &= ~mbi(bit))     // Poner a 0 bit de reg 
#define tbi(reg,bit) (reg ^= mbi(bit))      // Cambiar de valor bit de reg 

void setup() {
  // put your setup code here, to run once:
  sbi(DDRA,PA2);
  sbi(DDRC,PC2);
  sbi(DDRH,PH5);
  sbi(PORTA,PA2);
  cbi(PORTC,PC2);
  sbi(PORTH,PH5);
}

void loop() {
  // put your main code here, to run repeatedly:

}
