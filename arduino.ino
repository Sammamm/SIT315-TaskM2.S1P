// Define the pin numbers for the LED and the potentiometer
const byte LED_PIN = 13;
const byte METER_PIN = A4;

void setup() {
  // Set the LED pin as an output and the potentiometer pin as an input
  pinMode(LED_PIN, OUTPUT);
  pinMode(METER_PIN, INPUT);
  
  // Begin serial communication at a baud rate of 9600
  Serial.begin(9600);
  
  // Read the initial frequency from the potentiometer and start the timer
  double initialFrequency = determinePotentiometerFrequency();
  activateTimer(initialFrequency);
}

void loop() {
  // Continuously monitor the potentiometer for changes in frequency
  double frequency = determinePotentiometerFrequency();
  activateTimer(frequency);
  
  // Delay the loop to prevent rapid iteration
  delay(2000);
}

void activateTimer(double timerFrequency) {
  // Disable interrupts to prevent interference with timer configuration
  noInterrupts();
  
  // Calculate the value for OCR1A (Output Compare Register A)
  unsigned long ocrValue = F_CPU / (2 * 1024 * timerFrequency) - 1;
  OCR1A = ocrValue;
  
  // Configure Timer1 for CTC (Clear Timer on Compare Match) mode
  TCCR1A = 0;
  TCCR1B = (1 << WGM12);
  TCCR1B |= (1 << CS12) | (1 << CS10);
  TIMSK1 |= (1 << OCIE1A);
  
  // Re-enable interrupts
  interrupts();
}

// Interrupt Service Routine for Timer1 compare match A
ISR(TIMER1_COMPA_vect) {
   // Toggle the state of the LED
   digitalWrite(LED_PIN, digitalRead(LED_PIN) ^ 1);
}

// Function to read the frequency from the potentiometer
double determinePotentiometerFrequency() {
  int potValue = analogRead(METER_PIN);
  
  // Map the potentiometer value to a desired frequency range
  double frequency = map(potValue, 0, 1023, 0.1, 10.0);
  
  Serial.println(frequency);
  
  return frequency;
}
