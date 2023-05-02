
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <SD.h>
#include <Adafruit_VS1053.h>

//#ifdef __AVR__
// #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
//#endif

#define VS1053_RESET   -1     // VS1053 reset pin (not used!)

#define VS1053_CS       8     // VS1053 chip select pin (output)
#define VS1053_DCS      10    // VS1053 Data/command select pin (output)
#define CARDCS          7     // Card chip select pin
#define VS1053_DREQ     9     // VS1053 Data request, ideally an Interrupt pin

#define LED_PIN 11
#define LED_COUNT 8

int fsrPin = A0;     // the FSR and 10K pulldown are connected to a0
int fsrPin2 = A1;     // the FSR and 10K pulldown are connected to a0
int fsrPin3 = A2;     // the FSR and 10K pulldown are connected to a0
int fsrPin4 = 13;     // the FSR and 10K pulldown are connected to a0
int fsrPin5 = 24;     // the FSR and 10K pulldown are connected to a0
int fsrPin6 = 25;     // the FSR and 10K pulldown are connected to a0
int fsrPin7 = 12;     // the FSR and 10K pulldown are connected to a0

int fsrReading;     // the analog reading from the FSR resistor divider
int fsrReading2;     // the analog reading from the FSR resistor divider
int fsrReading3;     // the analog reading from the FSR resistor divider
int fsrReading4;     // the analog reading from the FSR resistor divider
int fsrReading5;     // the analog reading from the FSR resistor divider
int fsrReading6;     // the analog reading from the FSR resistor divider
int fsrReading7;

int marker = 0;
int marker2 = 0;
int marker3 = 0;
int marker4 = 0;
int marker5 = 0;
int marker6 = 0;
int marker7 = 0;

int capTh = 30000;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_VS1053_FilePlayer musicPlayer = 
  Adafruit_VS1053_FilePlayer(VS1053_RESET, VS1053_CS, VS1053_DCS, VS1053_DREQ, CARDCS);
void setup(void) {
   Serial.begin(115200);

  // if you're using Bluefruit or LoRa/RFM Feather, disable the radio module
  //pinMode(8, INPUT_PULLUP);

  // Wait for serial port to be opened, remove this line for 'standalone' operation
  while (!Serial) { delay(1); }
  delay(500);
  Serial.println("\n\nAdafruit VS1053 Feather Test");
  
  if (! musicPlayer.begin()) { // initialise the music player
     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
     while (1);
  }

  Serial.println(F("VS1053 found"));
 
  musicPlayer.sineTest(0x44, 500);    // Make a tone to indicate VS1053 is working
  
  if (!SD.begin(CARDCS)) {
    Serial.println(F("SD failed, or not present"));
    while (1);  // don't do anything more
  }
  Serial.println("SD OK!");
  
  // list files
  printDirectory(SD.open("/"), 0);
  
  // Set volume for left, right channels. lower numbers == louder volume!
  musicPlayer.setVolume(3,3);
  
#if defined(__AVR_ATmega32U4__) 
  // Timer interrupts are not suggested, better to use DREQ interrupt!
  // but we don't have them on the 32u4 feather...
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
#else
  // If DREQ is on an interrupt pin we can do background
  // audio playing
  musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
#endif
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(5); // Set BRIGHTNESS to about 1/5 (max = 255)
  // Play a file in the background, REQUIRES interrupts!
  Serial.println(F("Playing full track 001"));
  musicPlayer.playFullFile("/track001.mp3");

  Serial.println(F("Playing track 002"));
  musicPlayer.startPlayingFile("/Long.mp3");
//  Serial.begin(115200);  
//  while (!Serial) { delay(1); }
//  delay(500);
//  if (!musicPlayer.begin()) { // initialise the music player
//     Serial.println(F("Couldn't find VS1053, do you have the right pins defined?"));
//     while (1);
//  }
//  Serial.println(F("VS1053 found"));
//  musicPlayer.sineTest(0x44, 500); 
//  if (!SD.begin(CARDCS)) {
//    Serial.println(F("SD failed, or not present"));
//    while (1);  // don't do anything more
//  }
//  Serial.println("SD OK!");
//  printDirectory(SD.open("/"), 0);
//
//  musicPlayer.setVolume(20,20);
//
//  #if defined(__AVR_ATmega32U4__) 
//    // Timer interrupts are not suggested, better to use DREQ interrupt!
//    // but we don't have them on the 32u4 feather...
//    musicPlayer.useInterrupt(VS1053_FILEPLAYER_TIMER0_INT); // timer int
//  #else
//    // If DREQ is on an interrupt pin we can do background
//    // audio playing
////    musicPlayer.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
//  #endif
////  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
////    clock_prescale_set(clock_div_1);
////  #endif
//  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//  strip.show();            // Turn OFF all pixels ASAP
//  strip.setBrightness(5); // Set BRIGHTNESS to about 1/5 (max = 255)
//  //musicPlayer.startPlayingFile("/TOM005.mp3");
//  //musicPlayer.stopPlaying();
//  musicPlayer.startPlayingFile("/Long.mp3");
//  //musicPlayer.pausePlaying(true);
}
 
void loop(void) {
  fsrReading = analogRead(fsrPin);  
  fsrReading2 = analogRead(fsrPin2);  
  fsrReading3 = analogRead(fsrPin3);  
  fsrReading4 = RCtime(fsrPin4);  
  fsrReading5 = RCtime(fsrPin5);  
  fsrReading6 = RCtime(fsrPin6);  
  fsrReading7 = RCtime(fsrPin7);  
  
  Serial.print("Analog reading = A0: ");
  Serial.print(fsrReading);
  Serial.print(" - A1: ");
  Serial.print(fsrReading2);
  Serial.print(" - A2: ");
  Serial.print(fsrReading3);
  Serial.print(" - 13: ");
  Serial.print(fsrReading4);
  Serial.print(" - 24: ");
  Serial.print(fsrReading5);
  Serial.print(" - 25: ");
  Serial.print(fsrReading6);
  Serial.print(" - 12: ");
  Serial.print(fsrReading7);
  Serial.print(" - MARKER: ");
  Serial.print(marker);
  Serial.print(" ");
  Serial.print(musicPlayer.stopped() ? "stopped" : "playing");
  Serial.print(" ");
  Serial.print(musicPlayer.playingMusic ? "playing" : "stopped");

//  if (Serial.available()) {
//    char c = Serial.read();
//    if (c == 's' && fsrReading2 > 150) {
//      musicPlayer.stopPlaying();
//    }
//  }


//  if (fsrReading > 1000) {
//    //musicPlayer.playFullFile("/CRASH002.mp3");
//    musicPlayer.sineTest(0x44, 500); 
//    colorWipe(strip.Color(255,   0,   0), 25); // Red
//    colorWipe(strip.Color(  0, 0,   0), 25); // Green
//    while(musicPlayer.playingMusic) {
//      delay(1000);
//      break;
//    }
//    if (fsrReading2 > 150) {
//      musicPlayer.stopPlaying();
//    } else {
//      delay(250);
//    }
//  }
//  if (fsrReading3 > 150) {
//    colorWipe(strip.Color(255,   0,   0), 25); // Red
//    colorWipe(strip.Color(  0, 0,   0), 25); // Green
//  }
  // MARKER 0
  if (fsrReading < 150 && marker == 1) {
    marker = 0;
  } else if (marker == 0 && fsrReading > 150) {
    marker = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/crash.mp3");
    colorWipe(strip.Color(160, 32, 240), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  // MARKER 1
  if (fsrReading2 < 150 && marker2 == 1) {
    marker2 = 0;
  } else if (marker2 == 0 && fsrReading2 > 150) {
    marker2 = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/floor.mp3");
    colorWipe(strip.Color(255, 0, 0), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  // MARKER 2
  if (fsrReading3 < 150 && marker3 == 1) {
    marker3 = 0;
  } else if (marker3 == 0 && fsrReading3 > 150) {
    marker3 = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/tom.mp3");
    colorWipe(strip.Color(0, 255, 0), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  // MARKER 3
  if (fsrReading4 > capTh-5000 && marker4 == 1) {
    marker4 = 0;
  } else if (marker4 == 0 && fsrReading4 < 5000) {
    marker4 = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/tom.mp3");
    colorWipe(strip.Color(0, 0, 255), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  // MARKER 4
  if (fsrReading5 > capTh-5000 && marker5 == 1) {
    marker5 = 0;
  } else if (marker5 == 0 && fsrReading5 < 5000) {
    marker5 = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/snare.mp3");
    colorWipe(strip.Color(255, 0, 255), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  // MARKER 5
  if (fsrReading6 > capTh-5000 && marker6 == 1) {
    marker6 = 0;
  } else if (marker6 == 0 && fsrReading6 < 5000) {
    marker6 = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/pedal.mp3");
    colorWipe(strip.Color(255, 255, 0), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  // MARKER 6
  if (fsrReading7 > capTh-5000 && marker7 == 1) {
    marker7 = 0;
  } else if (marker7 == 0 && fsrReading7 < 5000) {
    marker7 = 1;
    if (musicPlayer.playingMusic) {
      musicPlayer.stopPlaying();
    }
    musicPlayer.startPlayingFile("/ride.mp3");
    colorWipe(strip.Color(0, 255, 255), 10);
    colorWipe(strip.Color(0, 0, 0), 10);
  }
  
//  if (marker) {
//    if (musicPlayer.stopped()) {
//      musicPlayer.startPlayingFile("/track001.mp3");
//      //musicPlayer.pausePlaying(false);
//      Serial.print(" - PLAYING - ");
//      //colorWipe(strip.Color(255,   0,   0), 25); // Red
//      //colorWipe(strip.Color(  0, 0,   0), 25); // Green
//      delay(50);
//    }
//  }
 
//  if (fsrReading < 10) {
//    Serial.println(" - No pressure");
//  } else if (fsrReading < 50) {
//    Serial.println(" - Light touch");
//  } else if (fsrReading < 120) {
//    Serial.println(" - Light squeeze");
//    if (!musicPlayer.playingMusic) {
//      musicPlayer.startPlayingFile("/track002.mp3");
//      colorWipe(strip.Color(  255, 0,   0), 10); // Green
//      colorWipe(strip.Color(  0, 0,   0), 10); // Green
//
//      delay(50);
//    } else {
//      musicPlayer.stopPlaying();
//    }
//  } else if (fsrReading < 250) {
//    Serial.println(" - Medium squeeze");
//  } else {
//    Serial.println(" - Big squeeze");
//  }

  // Fill along the length of the strip in various colors...
//  if (fsrReading > 700) {
//    colorWipe(strip.Color(255,   0,   0), 50); // Red
//    colorWipe(strip.Color(  0, 0,   0), 50); // Green
//  }
//  colorWipe(strip.Color(255,   0,   0), 50); // Red
//  colorWipe(strip.Color(  0, 0,   0), 50); // Green
////  colorWipe(strip.Color(  0,   0, 255), 5); // Blue

  // Do a theater marquee effect in various colors...
//  theaterChase(strip.Color(127, 127, 127), 50); // White, half brightness
//  theaterChase(strip.Color(127,   0,   0), 50); // Red, half brightness
//  theaterChase(strip.Color(  0,   0, 127), 50); // Blue, half brightness

//  rainbow(10);             // Flowing rainbow cycle along the whole strip
//  theaterChaseRainbow(50); // Rainbow-enhanced theaterChase variant
  Serial.println("  ");
  delay(15);
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

//
///// File listing helper
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

int RCtime(int RCpin) {
  int reading = 0; 
  pinMode(RCpin, OUTPUT);
  digitalWrite(RCpin, LOW);

  pinMode(RCpin, INPUT);
  while (digitalRead(RCpin) == LOW) {
    reading++;  
    if (reading == capTh) {
      break;
    }
  }
  return reading;
}
