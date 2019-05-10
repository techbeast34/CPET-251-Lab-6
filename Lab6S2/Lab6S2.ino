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

  DDRC |= 0x30; //Set PC4 and PC5 to outputs
  
  
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

void display_color_on_RGB_led(unsigned long color) {
  unsigned long bitmask=0UL; // UL unsigned long literal (forces compiler to use long data type)
  unsigned long masked_color_result=0UL;
  
  PORTC &= 0xDF; //start with clock low.
  
  for(int i=23; i>=0; i--) { // clock out one data bit at a time, starting with the MSB first
    bitmask= (1UL<<i);    // build bit mask. Note must use "1UL" unsigned long literal, not "1"
    masked_color_result = color & bitmask; // reveals just one bit of color at time
    boolean data_bit=!(masked_color_result==0); // this is the bit of data to be clocked out.
    //digitalWrite(LED_DATA_PIN,data_bit);
    
    if(data_bit){ PORTC |= 0x10; }
    else{ PORTC &= 0xEF; }
    
    //digitalWrite(LED_CLOCK_PIN,HIGH);
    PORTC |= 0x20;  
    //digitalWrite(LED_CLOCK_PIN,LOW);
    PORTC &= 0xDF;
  }
  //digitalWrite(LED_CLOCK_PIN,HIGH);  
  delay(1); // after writing data to LED driver, must hold clock line  
            // high for 1 ms to latch color data in led shift register.
}//display_color_on_RGB_led()

void loop() {
}
