#include <FastLED.h>

#define LED_RED 9
#define LED_GREEN 10
#define LED_BLUE 11

#define DATA_PIN 7 // Data pin for strips

#define NUM_LEDS 50

#define DOWN 4
#define UP 3
#define MODE 2 // Controller Pin

CRGB leds[NUM_LEDS];

int color = 0; // Value from 0 to 1530
int mode = 1; // 0 = manual cycle, 1 = autocycle, 2 = white (with brightness)
int brightness = 255; // for white mode 
int changeSpeed = 5; // for autocycle

void setup() {
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(MODE, INPUT);
  
  FastLED.addLeds<WS2811, DATA_PIN, BRG>(leds, NUM_LEDS);

  setColor(255,255,255);
}

void setColor(int Red, int Green, int Blue) {
  // LED Lights
  analogWrite(LED_RED, Red);
  analogWrite(LED_GREEN, Green);
  analogWrite(LED_BLUE, Blue);

  // LED Strips  
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

void checkModeButton() {
  if(digitalRead(MODE) == HIGH) {
    mode = (mode + 1) % 3; // Number of modes
    setColor(0,0,0);
    while(digitalRead(MODE) == HIGH) {}
    // Display the mode change
    for(int i = 0; i < 25; i++) {
      setColor(i*10, 0, i*9);
      delay(10);
    }
    for(int i = 25; i >=0; i--) {
      setColor(i*10, 0, i*9);
      delay(10);
    }
  }
}

void loop() {
  checkModeButton();
  
  if(mode == 0) {
    if(digitalRead(UP) == HIGH) {
      color++;
      if(color > 1530) {
        color-=1530; // Underflow
      }
    }
    else if(digitalRead(DOWN) == HIGH) {
      color--;
      if(color < 0) {
       color+=1530; // Overflow
      }
    }
  
    displayColor();
    delay(10);
  }

  else if(mode == 1) {
    // Change the speed
    if(digitalRead(UP) == HIGH && changeSpeed > 1) {
      changeSpeed--;
      delay(200);
    }
    else if(digitalRead(DOWN) == HIGH && changeSpeed < 100) {
      changeSpeed++;
      delay(200);
    }

    color = (color + 1) % 1530;
    displayColor();
    delay(changeSpeed);
  }

  else if(mode == 2) {  
    if(digitalRead(UP) == HIGH) {
      brightness+=10;
      if(brightness > 255) {
        brightness = 255;
      }
      delay(50);
    }
    else if(digitalRead(DOWN) == HIGH) {
      brightness-=10;
      if(brightness < 0) {
        brightness = 0;
      }
      delay(50);
    }

    setColor(brightness, brightness, brightness);
  }
}
