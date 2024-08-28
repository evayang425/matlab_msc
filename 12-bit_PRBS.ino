uint16_t lfsr = 0xFFF; // Initial state 111111111111
volatile boolean updateFlag = false;

void setClockFrequency(long hertz) {
  noInterrupts(); // Disable interrupts

  // Clear Timer1 control registers
  TCCR1A = 0;
  TCCR1B = 0;

  // Calculate compare match register value
  long compareMatch = (2000000 / hertz) - 1; 
  OCR1A = compareMatch;

  // Set CTC (Clear Timer on Compare) mode
  TCCR1B |= (1 << WGM12);

  // Set 8 prescaler
  TCCR1B |= (1 << CS11);

  // Enable Timer1 compare interrupt
  TIMSK1 |= (1 << OCIE1A);

  interrupts(); // Enable interrupts
}

ISR(TIMER1_COMPA_vect) {
  // Calculate the next value of the PRBS
  uint16_t nextBit11 = bitRead(lfsr, 0) ^ bitRead(lfsr, 1) ^ bitRead(lfsr, 2) ^ bitRead(lfsr, 8); // Next state bit 11
  uint16_t nextBit10 = bitRead(lfsr, 11); // Next state bit 10
  uint16_t nextBit9 = bitRead(lfsr, 10); // Next state bit 9
  uint16_t nextBit8 = bitRead(lfsr, 9); // Next state bit 8
  uint16_t nextBit7 = bitRead(lfsr, 8); // Next state bit 7
  uint16_t nextBit6 = bitRead(lfsr, 7); // Next state bit 6
  uint16_t nextBit5 = bitRead(lfsr, 6); // Next state bit 5
  uint16_t nextBit4 = bitRead(lfsr, 5); // Next state bit 4
  uint16_t nextBit3 = bitRead(lfsr, 4); // Next state bit 3
  uint16_t nextBit2 = bitRead(lfsr, 3); // Next state bit 2
  uint16_t nextBit1 = bitRead(lfsr, 2); // Next state bit 1
  uint16_t nextBit0 = bitRead(lfsr, 1); // Next state bit 0

  // Generate the next state
  lfsr = (nextBit11 << 11) | (nextBit10 << 10) | (nextBit9 << 9) | (nextBit8 << 8) |
         (nextBit7 << 7) | (nextBit6 << 6) | (nextBit5 << 5) | (nextBit4 << 4) |
         (nextBit3 << 3) | (nextBit2 << 2) | (nextBit1 << 1) | nextBit0;
    
  updateFlag = true; // Set flag indicating Timer interrupt occurred
}

void setup() {
  //Serial.begin(9600); // Initialize serial communication for viewing PRBS value in Serial Monitor
  pinMode(10, INPUT);
  pinMode(13, OUTPUT); // Set digital pin 13 as output
  setClockFrequency(50); // Set clock frequency
}

void loop() {
  if (updateFlag) {
    updateFlag = false;
    if(digitalRead(10) == HIGH) {
      digitalWrite(13, bitRead(lfsr, 0)); // Set pin 13 state to the current state bit 0
    } else {
      digitalWrite(13, LOW);
    }
  }
}
