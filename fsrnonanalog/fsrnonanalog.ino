/* FSR simple testing sketch. 
 
Connect one end of FSR to power, the other end to pin 2.
Then connect one end of a 0.1uF capacitor from pin 2 to ground 
 
For more information see www.ladyada.net/learn/sensors/fsr.html */
 
int fsrPin = 12;     // the FSR and cap are connected to pin2
int fsrReading;     // the digital reading
int ledPin = 13;    // you can just use the 'built in' LED
 
void setup(void) {
  // We'll send debugging information via the Serial monitor
  Serial.begin(9600);   
  pinMode(ledPin, OUTPUT);  // have an LED for output 
}
 
void loop(void) {
  // read the resistor using the RCtime technique
  fsrReading = RCtime(fsrPin);
  Serial.print("RCtime (BEFORE) reading = ");
  Serial.println(fsrReading);     // the raw analog reading
 
  if (fsrReading == 100000) {
    // if we got 30000 that means we 'timed out'
    Serial.println("Nothing connected!");
  } else {
    Serial.print("RCtime reading = ");
    Serial.println(fsrReading);     // the raw analog reading
 
    // Do a little processing to keep the LED blinking
    fsrReading /= 10;
    // The more you press, the faster it blinks!
    digitalWrite(ledPin, HIGH);
    delay(fsrReading);
    digitalWrite(ledPin, LOW);
    delay(fsrReading);
  }
  delay(100);
}
 
// Uses a digital pin to measure a resistor (like an FSR or photocell!)
// We do this by having the resistor feed current into a capacitor and
// counting how long it takes to get to Vcc/2 (for most arduinos, thats 2.5V)
int RCtime(int RCpin) {
 int reading = 0;  // start with 0
 
  // set the pin to an output and pull to LOW (ground)
  pinMode(RCpin, OUTPUT);
  digitalWrite(RCpin, LOW);
 
  // Now set the pin to an input and...
  pinMode(RCpin, INPUT);
  while (digitalRead(RCpin) == LOW) { // count how long it takes to rise up to HIGH
    reading++;      // increment to keep track of time 
 
    if (reading == 100000) {
      // if we got this far, the resistance is so high
      // its likely that nothing is connected! 
      break;           // leave the loop
    }
  }
  // OK either we maxed out at 30000 or hopefully got a reading, return the count
 
  return reading;
}
