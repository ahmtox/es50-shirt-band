
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>
//#include "protothreads.h"

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

#define VS1053_CS       8     // VS1053 chip select pin (output)
#define VS1053_DCS      10    // VS1053 Data/command select pin (output)
#define CARDCS          7     // Card chip select pin
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin

#define LED_PIN A1
#define LED_COUNT 8

int fsrPin = A0;     // the FSR and 10K pulldown are connected to a0
int fsrPin2 = A2;     // the FSR and 10K pulldown are connected to a0
int fsrPin3 = A3;     // the FSR and 10K pulldown are connected to a0
int fsrPin4 = 24;     // the FSR and 10K pulldown are connected to a0
int fsrPin5 = 25;     // the FSR and 10K pulldown are connected to a0
int fsrPin6 = 13;     // the FSR and 10K pulldown are connected to a0

int fsrReading;     // the analog reading from the FSR resistor divider
int fsrReading2;     // the analog reading from the FSR resistor divider
int fsrReading3;     // the analog reading from the FSR resistor divider
int fsrReading4;     // the analog reading from the FSR resistor divider
int fsrReading5;     // the analog reading from the FSR resistor divider
int fsrReading6;     // the analog reading from the FSR resistor divider

int marker = false;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_VS1053_FilePlayer musicPlayer = Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);

//int playSound(struct pt* pt) {
//  PT_BEGIN(pt);
//
//  while (true) {
//    digitalWrite(
//  }
//}
 
void setup(void) {
  Serial.begin(9600);  
  Serial1.begin(115200); 
  while (!Serial1) { delay(1); }

  if (!musicPlayer.begin()) { // initialise the music player
     Serial1.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }
  Serial1.println(F("VS1053 found"));
  if (!SD.begin(CARDCS)) {
    Serial1.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial1.println("SD OK!");
  //printDirectory(SD.open("/"), 0);

  musicPlayer.setVolume(10,10);

  #if defined(__AVR_ATmega32U4__) 
    // Timer interrupts are not suggested, better to use DREQ interrupt!
    // but we don't have them on the 32u4 feather...
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
  #else
    // If DREQ is on an interrupt pin we can do background
    // audio playing
    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
  #endif
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
  #endif
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(5); // Set BRIGHTNESS to about 1/5 (max = 255)
}
 
void loop(void) {
  fsrReading = analogRead(fsrPin);  
  fsrReading2 = analogRead(fsrPin2);  
  fsrReading3 = analogRead(fsrPin3);  
  fsrReading4 = analogRead(fsrPin4);  
  fsrReading5 = analogRead(fsrPin5);  
  fsrReading6 = analogRead(fsrPin6);  
 
  Serial.print("Analog reading = ");
  Serial.print(fsrReading + " ");     // print the raw analog reading
  Serial.print(fsrReading2 + " ");
  Serial.print(fsrReading3 + " ");
  Serial.print(fsrReading4 + " ");
  Serial.print(fsrReading5 + " ");
  Serial.print(fsrReading6);
 
  if (fsrReading < 10) {
    Serial.println(" - No pressure");
  } else if (fsrReading < 50) {
    Serial.println(" - Light touch");
  } else if (fsrReading < 120) {
    Serial.println(" - Light squeeze");
    if (!musicPlayer.playingMusic) {
      musicPlayer.startPlayingFile("/track002.mp3");
      colorWipe(strip.Color(  255, 0,   0), 10); // Green
       colorWipe(strip.Color(  0, 0,   0), 10); // Green

      delay(50);
    } else {
      musicPlayer.stopPlaying();
    }
  } else if (fsrReading < 250) {
    Serial.println(" - Medium squeeze");
  } else {
    Serial.println(" - Big squeeze");
  }

  // Fill along the length of the strip in various colors...
  if (fsrReading > 700) {
    colorWipe(strip.Color(255,   0,   0), 50); // Red
    colorWipe(strip.Color(  0, 0,   0), 50); // Green
  }
//  colorWipe(strip.Color(255,   0,   0), 50); // Red
//  colorWipe(strip.Color(  0, 0,   0), 50); // Green
////  colorWipe(strip.Color(  0,   0, 255), 5); // Blue

  // Do a theater marquee effect in various colors...
//  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
//  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

//  rainbow(10);             // Flowing rainbow cycle along the whole strip
//  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

void shockwave(uint32_t color, int wait) {
  for (int i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, color);
    strip.show();
    delay(wait);
  }
}

// Theater-marquee-style chasing lights. Pass in a color (32-bit value,
// a la strip.Color(r,g,b) as mentioned above), and a delay time (in ms)
// between frames.
//void theaterChase(uint32_t color, int wait) {
//  for(int a=0; a<10; a++) {  // Repeat 10 times...
//    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//      strip.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in steps of 3...
//      for(int c=b; c<strip.numPixels(); c += 3) {
//        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      strip.show(); // Update strip with new contents
//      delay(wait);  // Pause for a moment
//    }
//  }
//}
//
//// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
//void rainbow(int wait) {
//  // Hue of first pixel runs 5 complete loops through the color wheel.
//  // Color wheel has a range of 65536 but it's OK if we roll over, so
//  // just count from 0 to 5*65536. Adding 256 to firstPixelHue each time
//  // means we'll make 5*65536/256 = 1280 passes through this outer loop:
//  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
//    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
//      // Offset pixel hue by an amount to make one full revolution of the
//      // color wheel (range of 65536) along the length of the strip
//      // (strip.numPixels() steps):
//      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
//      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
//      // optionally add saturation and value (brightness) (each 0 to 255).
//      // Here we're using just the single-argument hue variant. The result
//      // is passed through strip.gamma32() to provide 'truer' colors
//      // before assigning to each pixel:
//      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
//    }
//    strip.show(); // Update strip with new contents
//    delay(wait);  // Pause for a moment
//  }
//}
//
//// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
//void theaterChaseRainbow(int wait) {
//  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
//  for(int a=0; a<30; a++) {  // Repeat 30 times...
//    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
//      strip.clear();         //   Set all pixels in RAM to 0 (off)
//      // 'c' counts up from 'b' to end of strip in increments of 3...
//      for(int c=b; c<strip.numPixels(); c += 3) {
//        // hue of pixel 'c' is offset by an amount to make one full
//        // revolution of the color wheel (range 65536) along the length
//        // of the strip (strip.numPixels() steps):
//        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
//        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
//        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
//      }
//      strip.show();                // Update strip with new contents
//      delay(wait);                 // Pause for a moment
//      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
//    }
//  }
//}


/// File listing helper
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
}
