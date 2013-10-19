// For Arduino UNO, only pins 2 & 3 work for interrupts
int _encoderDialPin1 = 2;
int _encoderDialPin2 = 3;
int _encoderBtnPin = 4;
int _ledPin = 13;

volatile int _changeInTicks = 0;

// Don't need to be volatile since only accessed from single interrupt handler
int _lastBit1 = 0;
int _lastBit2 = 0;

void setup() {
  Serial.begin (9600);

  pinMode(_encoderDialPin1, INPUT); 
  pinMode(_encoderDialPin2, INPUT);
  pinMode(_encoderBtnPin, INPUT);
  pinMode(_ledPin, OUTPUT);

  // Turn on pullup resistor
  digitalWrite(_encoderDialPin1, HIGH);
  digitalWrite(_encoderDialPin2, HIGH);
  digitalWrite(_encoderBtnPin, HIGH);
  
  digitalWrite(_ledPin, LOW);
  
  attachInterrupt(0, handleEncoderChange, CHANGE); 
  attachInterrupt(1, handleEncoderChange, CHANGE);
}

void loop() {
  int decompressed = digitalRead(_encoderBtnPin);
  if (decompressed) {
    digitalWrite(_ledPin, LOW);
  } else {
    digitalWrite(_ledPin, HIGH);
  }
  
  int changeInTicks = 0;
  noInterrupts();
  changeInTicks = _changeInTicks;
  _changeInTicks = 0;
  interrupts();
  
  if (changeInTicks != 0) {
    Serial.println(changeInTicks);
  }
  
  delay(250);
}


void handleEncoderChange() {
  int bit1 = digitalRead(_encoderDialPin1);
  int bit2 = digitalRead(_encoderDialPin2);

  int code = (_lastBit1 << 3) | (_lastBit2 << 2) | (bit1 << 1) | bit2;
  
  if (code == 0b0001 || code == 0b0111 || code == 0b1110 || code == 0b1000) {
    _changeInTicks++;
  } else if (code == 0b1011 || code == 0b1101 || code == 0b0100 || code == 0b0010) {
    _changeInTicks--;
  } else {
    // For this case, the direction of the turn is indeterminate
  }
  
  _lastBit1 = bit1;
  _lastBit2 = bit2;
}
