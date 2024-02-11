// Define pin numbers
const byte LED_PIN = 13;
const byte METER_PIN = A4;
// Initialize previous timer frequency variable
double prevTimerFrequency = 0;

void setup() {
  // Set pin modes
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read analog value from sensor pin
  int read_value = analogRead(METER_PIN);
  // Map the read value to a timer frequency range
  double timerFrequency = map(read_value, 0, 1023, 1, 10);

  // Check if timer frequency has changed
  if (timerFrequency != prevTimerFrequency) {
    // Print the new timer frequency
    Serial.println(timerFrequency);
    // Start timer with the new frequency
    startTimer(timerFrequency);
    // Update previous timer frequency
    prevTimerFrequency = timerFrequency;
  }

  // Delay loop to avoid excessive readings
  delay(200);
}

// Function to start timer with given frequency
void startTimer(double timerFrequency) {
  // Disable interrupts during timer setup
  noInterrupts();
  // Timer1 setup for 2-second interval
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 16000000 / 1024 * timerFrequency; // Compare match value based on timer frequency 
  TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Configure for CTC mode with prescaler 1024 
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare match A interrupt
  // Enable interrupts after timer setup
  interrupts();
}

// Interrupt service routine for Timer1 compare match A
ISR(TIMER1_COMPA_vect) {
  // Toggle LED pin state
  digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
}
