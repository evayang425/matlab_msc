uint8_t lfsr = 0x3F; // Initial state 111111
volatile boolean updateFlag = false;

void setClockFrequency(long hertz) {
  noInterrupts(); // Disable interrupts

  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Set compare match register value
  //long compareMatch = (2000000 / hertz) - 1; 

  // when set clock frequency 1Hz
  long compareMatch = (15625 / hertz) - 1;
  
  OCR1A = compareMatch;

  // Set CTC (Clear Timer on Compare) mode
  TCCR1B |= (1 << WGM12);

  // Set 8 prescaler
  //TCCR1B |= (1 << CS11);

  // 1Hz
  TCCR1B |= (1 << CS10) | (1 << CS12);

  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
  // Calculate the next value of the PRBS
  uint8_t nextBit5 = bitRead(lfsr, 0) ^ bitRead(lfsr, 1); // Next state bit 5
  uint8_t nextBit4 = bitRead(lfsr, 5); // Next state bit 4
  uint8_t nextBit3 = bitRead(lfsr, 4); // Next state bit 3
  uint8_t nextBit2 = bitRead(lfsr, 3); // Next state bit 2
  uint8_t nextBit1 = bitRead(lfsr, 2); // Next state bit 1
  uint8_t nextBit0 = bitRead(lfsr, 1); // Next state bit 0

  // Generate the next state 
  lfsr = (nextBit5 << 5) | (nextBit4 << 4) | (nextBit3 << 3) | (nextBit2 << 2) | (nextBit1 << 1) | nextBit0;

  updateFlag = true; // Set flag indicating Timer interrupt occurred
}

void setup() {
  //Serial.begin(9600); // Initialize serial communication for viewing PRBS value in Serial Monitor
  pinMode(10, INPUT);
  pinMode(13, OUTPUT); // Set digital pin 13 as output
  setClockFrequency(1); // Set clock frequency
}

void loop() { 
  if (updateFlag) {
    updateFlag = false;
    
    if(digitalRead(10) == HIGH) {
      digitalWrite(13, bitRead(lfsr, 0));
    } else {
      digitalWrite(13, LOW);
    }
  }
}
