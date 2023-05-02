//  File:  blinkNeoPixel.ino
//
//  Blink the single built-in NeoPixel on a Feather nRF42840.
//  This code is a highly stripped down and simplified version of
//  neopixel.ino from the Adafruit Bluefruit nRF52 Library example

#include <Adafruit_NeoPixel.h>    //  Library that provides NeoPixel functions

// -- Create a NeoPixel object called onePixel that addresses 1 pixel in pin 8
Adafruit_NeoPixel onePixel = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);

void setup()  {
  onePixel.begin();             // Start the NeoPixel object
  onePixel.clear();             // Set NeoPixel color to black (0,0,0)
  onePixel.setBrightness(20);   // Affects all subsequent settings
  onePixel.show();              // Update the pixel state
}

void loop()  {

  int r=200, g=0, b=100;    //  Red, green and blue intensity to display
  
  onePixel.setPixelColor(0, r, g, b);   //  Set pixel 0 to (r,g,b) color value
  onePixel.show();                      //  Update pixel state
  delay(1000);

  // onePixel.setPixelColor(0, 0, 0, 0);  //  same effect as clear()
  onePixel.clear();                       //  Set pixel to black (0,0,0)
  onePixel.show();                        //  Update pixel state
  delay(1000);
}
