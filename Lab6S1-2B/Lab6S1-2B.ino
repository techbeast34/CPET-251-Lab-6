//Lab6_section2_timer0_starter.ino
void setup() {
  Serial.begin(9600);
  Serial.println(F("Testing motor A and B using Timer0 in fast PWM mode 3."));
  Serial.println(F("Requires external 9V battery pack.")); 
  
  Serial.print(F("TCCR0A: 0b"));
  Serial.println(TCCR0A, BIN);

  Serial.print(F("TCCR0B: 0b"));
  Serial.println(TCCR0B, BIN);

  Serial.print(F("OCR0A: 0b"));
  Serial.println(OCR0A, BIN);

  Serial.print(F("OCR0B: 0b"));
  Serial.println(OCR0B, BIN);

  Serial.println(F("Configuring Timer0"));
  configureTimer0RegisterForPWMtoDriveMotor();
  Serial.println(F("Timer0 configured"));
  
  
// add your code here, no digitalWrite() or pinMode() commands
}

void configureTimer0RegisterForPWMtoDriveMotor(){
  TCCR0A |= 0xA3;
  TCCR0A &= 0xAF;
  TCCR0B &= 0xF7;
  //Set timer0 to mode 3 (fast PWM) and set COM bits to clear on compare

  OCR0A = 120;
  OCR0B = 120;

  DDRD |= 0xE0;
  DDRB |= 0x07;

  PORTB |= 0x02;
  PORTB &= 0xFB;
  PORTD |= 0x80;
   
  Serial.print(F("TCCR0A: 0b"));
  Serial.println(TCCR0A, BIN);

  Serial.print(F("TCCR0B: 0b"));
  Serial.println(TCCR0B, BIN);

  Serial.print(F("OCR0A: 0b"));
  Serial.println(OCR0A, BIN);

  Serial.print(F("OCR0B: 0b"));
  Serial.println(OCR0B, BIN);

  Serial.print(F("DDRD: 0b"));
  Serial.println(DDRD, BIN);

  Serial.print(F("DDRB: 0b"));
  Serial.println(DDRB, BIN);

  Serial.print(F("PORTD: 0b"));
  Serial.println(PORTD, BIN);

  Serial.print(F("PORTB: 0b"));
  Serial.println(PORTB, BIN);
}

void loop() {
// no code at all in main loop
}
