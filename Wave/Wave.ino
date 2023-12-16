#include <FastLED.h>

#define NUM_LEDS 15
//#define DATA_PIN_DASH 6
#define DATA_PIN_FOOTWELL 7 // Data pin for strips

// Define the array of leds
CRGB leds[NUM_LEDS];
int color = 1;

void setup() { 
//   FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);
  FastLED.addLeds<WS2811, DATA_PIN_FOOTWELL, BRG>(leds, 0, 17);
//  FastLED.addLeds<WS2811, DATA_PIN_DASH, BRG>(leds, 16, 25);
}

void setColor(int Red, int Green, int Blue) {
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(Red, Green, Blue);
  }
  FastLED.show();
}

void displayColor() {
  if(color >= 0 && color <= 255) {
    setColor(255, 0, color);
  }
  else if(color > 255 && color <= 510) {
    setColor(510-color, 0, 255);
  }
  else if(color > 510 && color <= 765) {
    setColor(0, color-510, 255);
  }
  else if(color > 765 && color <= 1020) {
    setColor(0, 255, 1020-color);
  }
  else if(color > 1020 && color <= 1275) {
    setColor(color-1020, 255, 0);
  }
  else if(color > 1275 && color <= 1530) {
    setColor(255, 1530-color, 0);
  }
}

void loop() { 
// 0-4 pass
// 5-9 driver
// 10-13 under pass
// 14-17 under driver

// 0 clock pod
// 1 temperature guage
// 2 tacometer
// 3 speedometer
// 4 fuel guage
// 5 centre of guages
// 6 centre of guages
// 7 odometer
//
//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(255, 0, 0);
//  FastLED.show();
//  delay(100);
//}
//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(0, 255, 0);
//  FastLED.show();
//  delay(100);
//}
//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(0, 0, 255);
//  FastLED.show();
//  delay(100);
//}
//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(0, 255, 255);
//  FastLED.show();
//  delay(100);
//}
//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(255, 0, 255);
//  FastLED.show();
//  delay(100);
//}
//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(255, 255, 0);
//  FastLED.show();
//  delay(100);
//}

//for(int i = 0; i < 7; i++) {
//  leds[i].setRGB(255, 0, 0);
//  FastLED.show();
//  delay(100);
//}


//  color++;
//  if(color >= 1530) {
//    color-=1530;
//  }
//  displayColor();
//  delay(10);
  
//  // Turn the LED on, then pause
//  for(int i = 0; i < NUM_LEDS; i++) {
//    leds[i].setRGB(255,255,255);
//  }
//  FastLED.show();
//  delay(500);
//  // Now turn the LED off, then pause
//  for(int i = 0; i < NUM_LEDS; i++) {
//    leds[i].setRGB(0,0,0);
//  }
//  FastLED.show();
//  delay(500);

  
//  for(int i = 0; i < 4; i++) {
//    leds[i] = CRGB::Red; //passenger LED 0-5
//    leds[i+5] = CRGB::Red; //driver
////    if(i != 5) {
////      leds[i+10] = CRGB::Red; //under passenger
////    }
////    leds[i+13] = CRGB::Red; //under driver
//    FastLED.show();
//    delay(100);
//  }
//  for(int i = 4; i >= 0; i--) {
//    leds[i] = CRGB::Blue; //passenger
//    leds[i+5] = CRGB::Blue; //driver
////    if(i != 5) {
////      leds[i+10] = CRGB::Blue; //under passenger
////      leds[i+14] = CRGB::Blue; //under driver
////    }
//    FastLED.show();
//    delay(100);
//  }

//  for(int i = 0; i < 18; i++) {
//    leds[i] = CRGB::Blue;
//    FastLED.show();
//    delay(100);
//  }
//  for(int i = 18; i >= 0; i--) {
//    leds[i] = CRGB::Green;
//    FastLED.show();
//    delay(100);
//  }




// Signal light
//  // Off
//  for(int i = 0; i < 5; i++) {
//    leds[i].setRGB(0, 0, 0);
//  }
//  FastLED.show();
//
//  // On
//  for(int i = 0; i < 5; i++) {
//    leds[i].setRGB(255, 50, 0);
//    FastLED.show();
//    delay(135);
//  }


//  for(int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = CRGB::Green;
//    FastLED.show();
//    delay(10);
//  }
//
//  for(int i = NUM_LEDS; i >= 0; i--) {
//    leds[i] = CRGB::Red;
//    FastLED.show();
//    delay(10);
//  }
//  for(int i = 0; i < NUM_LEDS; i++) {
//    leds[i] = CRGB::Blue;
//    FastLED.show();
//    delay(10);
//  }
//  for(int i = NUM_LEDS; i >= 0; i--) {
//    leds[i] = CRGB::Green;
//    FastLED.show();
//    delay(10);
//  }




//  for(int i = NUM_LEDS; i >= 0; i--) {
//    leds[i].setRGB(0,255,0);
//  }
//  FastLED.show();




//  for(int i = 0; i < NUM_LEDS; i++) {
//    leds[i-1] = CRGB::Black;
//    leds[i] = CRGB::White;
//    delay(20);
//    FastLED.show();
//  }
//  for(int i = NUM_LEDS; i >= 0; i--) {
//    leds[i+1] = CRGB::Black;
//    leds[i] = CRGB::White;
//    delay(20);
//    FastLED.show();
//  }

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(255, 50, 0);
    FastLED.show();
    delay(100);
  }

  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i].setRGB(0, 0, 0);
    FastLED.show();
    delay(60);
  }
  
}
