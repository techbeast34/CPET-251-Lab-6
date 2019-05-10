//Lab6_section2_timer0_starter.ino

#define DIM_GREEN 0x00001F
#define DIM_WHITE 0x1F1F1F
#define DIM_GREEN_BLUE 0X001F1F
#define DIM_RED_BLUE 0X1F1F00
#define DIM_RED 0X1F0000

enum directionState_t {STOPPED, FORWARD, BACKWARD, LEFT_TURN, RIGHT_TURN};

directionState_t directionState = FORWARD;
directionState_t previousDirectionState = -1;
int stateTimer = 0;
int timeStampStartOfLoopMs = 0;
boolean isNewState = true;
int counter = 0;

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

void go_forward(int rate) {
  PORTB |= 0x02;
  PORTB &= 0xFB;
  PORTD |= 0x80;
  OCR0A = rate;
  OCR0B = rate;
  display_color_on_RGB_led(DIM_GREEN);
}

void go_backward(int rate) {
  PORTB |= 0x05;
  PORTB &= 0xFD;
  PORTD &= 0x00;
  OCR0A = rate;
  OCR0B = rate;
  display_color_on_RGB_led(DIM_WHITE);
}

void turn_clockwise(int rate) {
  display_color_on_RGB_led(DIM_GREEN_BLUE);
  PORTB |= 0x03;
  PORTB &= 0xFB;
  PORTD &= 0x00;
  OCR0A = rate;
  OCR0B = rate;
}

void turn_counterclockwise(int rate) {
  display_color_on_RGB_led(DIM_RED_BLUE);
  PORTB |= 0x04;
  PORTB &= 0xFC;
  PORTD |= 0x80;
  OCR0A = rate;
  OCR0B = rate;
}

void stop_motor(int rate) {
  PORTD &= 0x00;
  PORTB &= 0xF8;
  display_color_on_RGB_led(DIM_RED);
  OCR0A = rate;
  OCR0B = rate; 
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
  timeStampStartOfLoopMs = millis();
  
  isNewState = (directionState != previousDirectionState);
  previousDirectionState = directionState;

  switch(directionState){
    case STOPPED:
      if(isNewState){
        //Housekeeping
        stateTimer = 0;
        stop_motor(0);
      }
      stateTimer++;
      if(stateTimer >= 150) directionState = TURN_LEFT;
      break;
    case FORWARD:
      if(isNewState){
        //Housekeeping
        stateTimer = 0;
        go_forward(120);
      }
      stateTimer++;
      if(stateTimer >= 150) directionState = STOPPED;
      break;
    case BACKWARD:
      if(isNewState){
        //Housekeeping
        stateTimer = 0;
        go_backward(120);
      }
      stateTimer++;
      if(stateTimer >= 500) directionState = LEFT_TURN;
      break;
    case LEFT_TURN:
      if(isNewState){
        //Housekeeping
        stateTimer = 0;
        turn_counterclockwise(120);
      }
      stateTimer++;
      
      if(stateTimer >= 150) {directionState = FORWARD};
      break;
    case RIGHT_TURN:
      if(isNewState){
        //Housekeeping
        stateTimer = 0;
        turn_clockwise(120);
      }
      stateTimer++;
      if(stateTimer >= 150) directionState = STOPPED;
      break;
    default: directionState = STOPPED;
  }

  delay(10-(millis() - timeStampStartOfLoopMs));
  
}
