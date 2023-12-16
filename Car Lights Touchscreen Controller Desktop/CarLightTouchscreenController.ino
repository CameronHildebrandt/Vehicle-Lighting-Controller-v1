// Include necessary libraries
#include <Arduino.h>                  // Arduino Library
#include <Adafruit_GFX.h>             // Core graphics library
#include <Adafruit_TFTLCD.h>          // Hardware-specific library
#include <TouchScreen.h>              // Touchscreen controller
#include <Fonts/FreeSansBold18pt7b.h> // Display font
#include <Fonts/FreeSans9pt7b.h>      // Display font
#include <FastLED.h>                  // Controller for Indiv. Addressable LEDs


// Display pins
#define YP A5 // Touchscreen
#define XM A4 // Touchscreen
#define YM 5 // Touchscreen
#define XP 4 // Touchscreen
#define LCD_CS A3 // Chip Select goes to Analog 3
#define LCD_CD A2 // Command/Data goes to Analog 2
#define LCD_WR A1 // LCD Write goes to Analog 1
#define LCD_RD A0 // LCD Read goes to Analog 0

#define LCD_RESET A6 // Useless Pin

// Light pins
#define LED_RED 11
#define LED_GREEN 12
#define LED_BLUE 13

#define DATA_PIN_DASH 6 // Data pin for strips
#define DATA_PIN_FOOTWELL 7 // Data pin for strips

#define NUM_LEDS 25

// Feed libraries the pins used
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
CRGB leds[NUM_LEDS];


// Global variables
int ControlScreenSectionNum = 0; // Section of lights to change
int CurrScreen = 0; // Screen the GUI is on
/* 0 == main
 * 1 == hide
 * 2 == control
 * 3 == wave */
int SectionMode[6] = {0,0,0,0,0,0}; // The mode that each section is on
/* 0 == cycle
 * 1 == RGB
 * 2 == disco */
int SectionRGB[6][3] = {
  {255,255,255}, // Dash
  {255,255,255}, // Climate Controls
  {255,255,255}, // Driver Front
  {255,255,255}, // Passenger Front
  {255,255,255}, // Driver Rear
  {255,255,255}  // Passenger Rear
};
int AllRGB[3] = {255,255,255};
const char *Section[7] = {
  "All",
  "Dash",
  "Climate Controls",
  "Driver Front",
  "Passenger Front",
  "Driver Rear",
  "Passenger Rear"
};
int FadeColor = 0; // Colour for the LEDs to fade through, value from 0-1530
int FadeSpeed = 1; // How much the FadeColor is incremented each cycle, value from 1 to 50
int FlashSpeed = 2000; // How often the Disco colours are updated, value from 100 to 4000
int Brightness = 100; // Value from 0 to 100
bool DiscoSync = true; // Makes all sections the same colour when on disoc mode
int LastDiscoMillis = 0; // Helps watch the clock
int ScreenTouched = 0; // So that toggle switches know what to do, 0 means no touch.
bool StartupFadeIn = true; // Fade from 0 brightness on startup


// Pre Declaring Functions
void welcomeFunction();
void drawHide(bool drawMode);
void drawControlScreen(bool drawMode);
void generateRBG();


void setup() {
  Serial.begin(9600);
  tft.reset();

  // Set Random Seed
  randomSeed(analogRead(0));

  // Enable LEDs
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);

  // Footwell
  FastLED.addLeds<WS2811, DATA_PIN_FOOTWELL, BRG>(leds, 0, 18);

  // Dash
  FastLED.addLeds<WS2811, DATA_PIN_DASH, BRG>(leds, 18, 25);

  // Initialize the display
  tft.begin(0x9341);
  tft.setRotation(1);
  tft.fillScreen(0x0000);

  drawMainScreen(true);

  // Welcome Pattern
  welcomeFunction();
  Brightness = 0;
}


// Wecome Function
void welcomeFunction() {
  int welcomePatternSpeed = 125;
  int red = 0;
  int blue = 0;
  int green = 0;

  // Blue Wave Out
    // Set to Blue
    red = 0; green = 0; blue = 255;

    // First
    leds[0].setRGB(red, green, blue); // Passenger
    leds[5].setRGB(red, green, blue); // Driver
    leds[10].setRGB(red, green, blue); // Passenger Rear
    leds[14].setRGB(red, green, blue); // Driver Rear
    leds[18].setRGB(red, green, blue); // Clock Pod

    // Dash
    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Second
    leds[1].setRGB(red, green, blue); // Passenger
    leds[6].setRGB(red, green, blue); // Driver
    leds[11].setRGB(red, green, blue); // Passenger Rear
    leds[15].setRGB(red, green, blue); // Driver Rear
    leds[19].setRGB(red, green, blue); // Temperature Guage

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Third
    leds[2].setRGB(red, green, blue); // Passenger
    leds[7].setRGB(red, green, blue); // Driver
    leds[12].setRGB(red, green, blue); // Passenger Rear
    leds[16].setRGB(red, green, blue); // Driver Rear
    leds[20].setRGB(red, green, blue); // Tacometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fourth
    leds[3].setRGB(red, green, blue); // Passenger
    leds[8].setRGB(red, green, blue); // Driver
    leds[13].setRGB(red, green, blue); // Passenger Rear
    leds[17].setRGB(red, green, blue); // Driver Rear
    leds[21].setRGB(red, green, blue); // Speedometer
    leds[23].setRGB(red, green, blue); // Speedometer
    leds[24].setRGB(red, green, blue); // Speedometer
    leds[25].setRGB(red, green, blue); // Speedometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fifth
    leds[4].setRGB(red, green, blue); // Passenger
    leds[9].setRGB(red, green, blue); // Driver
    leds[22].setRGB(red, green, blue); // Gas Guage

    // Update
    FastLED.show();
    delay(2 * welcomePatternSpeed);


  // Green Wave Back
    // Set to Green
    red = 0; green = 255; blue = 0;

    // First
    leds[4].setRGB(red, green, blue); // Passenger
    leds[9].setRGB(red, green, blue); // Driver
    leds[22].setRGB(red, green, blue); // Gas Guage

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Second
    leds[3].setRGB(red, green, blue); // Passenger
    leds[8].setRGB(red, green, blue); // Driver
    leds[13].setRGB(red, green, blue); // Passenger Rear
    leds[17].setRGB(red, green, blue); // Driver Rear
    leds[21].setRGB(red, green, blue); // Speedometer
    leds[23].setRGB(red, green, blue); // Speedometer
    leds[24].setRGB(red, green, blue); // Speedometer
    leds[25].setRGB(red, green, blue); // Speedometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Third
    leds[2].setRGB(red, green, blue); // Passenger
    leds[7].setRGB(red, green, blue); // Driver
    leds[12].setRGB(red, green, blue); // Passenger Rear
    leds[16].setRGB(red, green, blue); // Driver Rear
    leds[20].setRGB(red, green, blue); // Tacometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fourth
    leds[1].setRGB(red, green, blue); // Passenger
    leds[6].setRGB(red, green, blue); // Driver
    leds[11].setRGB(red, green, blue); // Passenger Rear
    leds[15].setRGB(red, green, blue); // Driver Rear
    leds[19].setRGB(red, green, blue); // Temperature Guage

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fifth
    leds[0].setRGB(red, green, blue); // Passenger
    leds[5].setRGB(red, green, blue); // Driver
    leds[10].setRGB(red, green, blue); // Passenger Rear
    leds[14].setRGB(red, green, blue); // Driver Rear
    leds[18].setRGB(red, green, blue); // Clock Pod

    // Dash
    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);

    // Update
    FastLED.show();
    delay(2 * welcomePatternSpeed);


  // Pink Wave From Dash
    // Set to Pink
    red = 255; green = 0; blue = 128;

    // First
    leds[21].setRGB(red, green, blue); // Speedometer
    leds[23].setRGB(red, green, blue); // Speedometer
    leds[24].setRGB(red, green, blue); // Speedometer
    leds[25].setRGB(red, green, blue); // Speedometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Second
    leds[22].setRGB(red, green, blue); // Fuel Gauge
    leds[20].setRGB(red, green, blue); // Tacometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Third
    leds[19].setRGB(red, green, blue); // Temperature Guage
    leds[7].setRGB(red, green, blue); // Centre of Driver

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fourth
    leds[18].setRGB(red, green, blue); // Clock Pod
    leds[8].setRGB(red, green, blue); // Centre of Driver
    leds[6].setRGB(red, green, blue); // Centre of Driver

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fifth
    leds[9].setRGB(red, green, blue); // Edge of Driver
    leds[5].setRGB(red, green, blue); // Edge of Driver
    leds[15].setRGB(red, green, blue); // Centre Under Driver

    // Dash
    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Sixth
    leds[0].setRGB(red, green, blue); // Passenger
    leds[16].setRGB(red, green, blue); // Centre Under Driver
    leds[14].setRGB(red, green, blue); // Edge Under Driver

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Seventh
    leds[1].setRGB(red, green, blue); // Passenger
    leds[17].setRGB(red, green, blue); // Edge Under Driver
    leds[10].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Eigth
    leds[2].setRGB(red, green, blue); // Passenger
    leds[11].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed/2);

    // Ninth
    leds[3].setRGB(red, green, blue); // Passenger
    leds[12].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed/2);


    // Tenth
    leds[4].setRGB(red, green, blue); // Passenger
    leds[13].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed/2);


  // Black Wave From Dash
    // Set to Black
    red = 0; green = 0; blue = 0;

    // First
    leds[21].setRGB(red, green, blue); // Speedometer
    leds[23].setRGB(red, green, blue); // Speedometer
    leds[24].setRGB(red, green, blue); // Speedometer
    leds[25].setRGB(red, green, blue); // Speedometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Second
    leds[22].setRGB(red, green, blue); // Fuel Gauge
    leds[20].setRGB(red, green, blue); // Tacometer

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Third
    leds[19].setRGB(red, green, blue); // Temperature Guage
    leds[7].setRGB(red, green, blue); // Centre of Driver

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fourth
    leds[18].setRGB(red, green, blue); // Clock Pod
    leds[8].setRGB(red, green, blue); // Centre of Driver
    leds[6].setRGB(red, green, blue); // Centre of Driver

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Fifth
    leds[9].setRGB(red, green, blue); // Edge of Driver
    leds[5].setRGB(red, green, blue); // Edge of Driver
    leds[15].setRGB(red, green, blue); // Centre Under Driver

    // Dash
    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Sixth
    leds[0].setRGB(red, green, blue); // Passenger
    leds[16].setRGB(red, green, blue); // Centre Under Driver
    leds[14].setRGB(red, green, blue); // Edge Under Driver

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Seventh
    leds[1].setRGB(red, green, blue); // Passenger
    leds[17].setRGB(red, green, blue); // Edge Under Driver
    leds[10].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed);


    // Eigth
    leds[2].setRGB(red, green, blue); // Passenger
    leds[11].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed/2);

    // Ninth
    leds[3].setRGB(red, green, blue); // Passenger
    leds[12].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed/2);


    // Tenth
    leds[4].setRGB(red, green, blue); // Passenger
    leds[13].setRGB(red, green, blue); // Under Passenger

    // Update
    FastLED.show();
    delay(welcomePatternSpeed/2);

  // Wait for a little
  delay(1000);
}


// Main Screen
void drawMainScreen(bool drawMode) {
  // drawMode == true, draw the screen
  // drawMode == false, clear the screen

  if(drawMode) {
    // Bounding Boxes
    tft.drawRoundRect(0, 0, 102, 75, 15, 0xF974);  // Dash
    tft.drawRoundRect(107, 0, 102, 75, 15, 0xF974);  // HVAC
    tft.drawRoundRect(214, 0, 102, 75, 15, 0xF974);  // Hide
    tft.drawRoundRect(0, 82, 102, 75, 15, 0xF974);  // DF
    tft.drawRoundRect(107, 82, 102, 75, 15, 0xF974);  // All
    tft.drawRoundRect(214, 82, 102, 75, 15, 0xF974);  // PF
    tft.drawRoundRect(0, 164, 102, 75, 15, 0xF974);  // DR
    tft.drawRoundRect(107, 164, 102, 75, 15, 0xF974);  // Wave
    tft.drawRoundRect(214, 164, 102, 75, 15, 0xF974);  // PR

    // Set the font and colour
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextColor(0x00FB);

    // First Row
    tft.setCursor(7, 47);
    tft.print("Dash");

    tft.setCursor(109, 47);
    tft.print("HVAC");

    tft.setCursor(227, 47);
    tft.print("Hide");

    // Second Row
    tft.setCursor(26, 131);
    tft.print("DF");

    tft.setCursor(135, 131);
    tft.print("All");

    tft.setCursor(242, 131);
    tft.print("PF");

    // Third Row
    tft.setCursor(24, 211);
    tft.print("DR");

    tft.setCursor(112, 211);
    tft.print("Wave");

    tft.setCursor(242, 211);
    tft.print("PR");
  }

  else {
    // Clear the Bounding Boxes
    tft.fillRoundRect(0, 0, 102, 75, 15, 0x0000);  // Dash
    tft.fillRoundRect(107, 0, 102, 75, 15, 0x0000);  // HVAC
    tft.fillRoundRect(214, 0, 102, 75, 15, 0x0000);  // Hide
    tft.fillRoundRect(0, 82, 102, 75, 15, 0x0000);  // DF
    tft.fillRoundRect(107, 82, 102, 75, 15, 0x0000);  // All
    tft.fillRoundRect(214, 82, 102, 75, 15, 0x0000);  // PF
    tft.fillRoundRect(0, 164, 102, 75, 15, 0x0000);  // DR
    tft.fillRoundRect(107, 164, 102, 75, 15, 0x0000);  // Wave
    tft.fillRoundRect(214, 164, 102, 75, 15, 0x0000);  // PR
  }
}


void mainScreenTouchCheck() {
  // Waits for a button to be pressed.
  TSPoint p = ts.getPoint();
  if(p.z > ts.pressureThreshhold) {

    // First Row
    if((p.x < 380) && (p.y > 630)) { // Dash

      // Highlight on Tap
      tft.fillRoundRect(0, 0, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 1; // Open the "Dash" screen
      drawControlScreen(true);
    }
    else if((p.x < 380) && (p.y > 350) && (p.y < 630)) { // HVAC

      // Highlight on Tap
      tft.fillRoundRect(107, 0, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 2; // Open the "HVAC" screen
      drawControlScreen(true);
    }
    else if((p.x < 380) && (p.y < 350)) { // Hide

      // Highlight on Tap
      tft.fillRoundRect(214, 0, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the hide screen
      CurrScreen = 1;
      drawHide(true);
    }

    // Second Row
    else if((p.x < 630) && (p.x > 380) && (p.y > 630)) { // DF

      // Highlight on Tap
      tft.fillRoundRect(0, 82, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 3; // Open the "Driver Front" screen
      drawControlScreen(true);
    }
    else if((p.x < 630) && (p.x > 380) && (p.y > 350) && (p.y < 630)) { // All

      // Highlight on Tap
      tft.fillRoundRect(107, 82, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 0; // Open the "All" screen
      drawControlScreen(true);
    }
    else if((p.x < 630) && (p.x > 380) && (p.y < 350)) { // PF

      // Highlight on Tap
      tft.fillRoundRect(214, 82, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 4; // Open the "Passenger Front" screen
      drawControlScreen(true);
    }

    // Third Row
    else if((p.x > 630) && (p.y > 630)) { // DR

      // Highlight on Tap
      tft.fillRoundRect(0, 164, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 5; // Open the "Driver Rear" screen
      drawControlScreen(true);
    }
    else if((p.x > 630) && (p.y > 350) && (p.y < 630)) { // Wave

      // Highlight on Tap
      tft.fillRoundRect(107, 164, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 3;
      drawWaveScreen(true);

    }
    else if((p.x > 630) && (p.y < 350)) { // PR

      // Highlight on Tap
      tft.fillRoundRect(214, 164, 102, 75, 15, 0x00FB);

      // Clear current screen
      drawMainScreen(false);

      // Go to the control screen
      CurrScreen = 2;
      ControlScreenSectionNum = 6; // Open the "Passenger Rear" screen
      drawControlScreen(true);
    }
  }
}


// Hide Screen
void drawHide(bool drawMode) {

  if(drawMode) {
    // Set the font and colour
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(0x7BEF);

    tft.setCursor(3, 237);
    tft.print("Tap to Show");
  }
  else {
    // Set the font and colour
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(0x0000);

    tft.setCursor(3, 237);
    tft.print("Tap to Show");
  }

}


void hideTouchCheck() {
  // Waits for screen to be tapped
  TSPoint p = ts.getPoint();
  if(p.z > ts.pressureThreshhold) {
    drawHide(false);

    // Go to the main screen
    CurrScreen = 0;
    drawMainScreen(true);
  }
}


// Control Screen
void drawControlScreen(bool drawMode) {

  if(drawMode) {
    // Back Button
    tft.fillTriangle(6, 20, 26, 6, 26, 33, 0xF974);
    tft.fillTriangle(15, 20, 26, 12, 26, 27, 0x0000);

    // Screen Title
    tft.setCursor(32, 32);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextColor(0x00FB);
    tft.print(Section[ControlScreenSectionNum]);

    // Dividers
    tft.fillRect(0, 40, 107, 2, 0xF974);
    tft.fillRect(0, 107, 107, 2, 0xF974);
    tft.fillRect(0, 174, 107, 2, 0xF974);
    tft.fillRect(0, 238, 107, 2, 0xF974);
    tft.fillRect(107, 40, 2, 200, 0xF974);
    tft.fillRect(0, 40, 2, 200, 0xF974);

    // Select the Current Mode
    if(ControlScreenSectionNum == 0) {
      /* If it's in "All" mode, check to see if all are the same, if they are
       * select that one, else don't select one.
       */
       if(SectionMode[0] == 0 &&
          SectionMode[1] == 0 &&
          SectionMode[2] == 0 &&
          SectionMode[3] == 0 &&
          SectionMode[4] == 0 &&
          SectionMode[5] == 0) { // All on cycle
         // Highlight Cycle
         tft.fillRect(0, 107, 107, 2, 0x00FB); // Top
         tft.fillRect(0, 174, 107, 2, 0x00FB); // Bottom
         tft.fillRect(107, 107, 2, 69, 0x00FB); // Right
         tft.fillRect(0, 107, 2, 67, 0x00FB); // Left

         drawCycleControlSubScreen(true, true, 2);
       }
       else if(SectionMode[0] == 1 &&
               SectionMode[1] == 1 &&
               SectionMode[2] == 1 &&
               SectionMode[3] == 1 &&
               SectionMode[4] == 1 &&
               SectionMode[5] == 1) { // All on RGB
         // Highlight RGB
         tft.fillRect(0, 40, 107, 2, 0x00FB); // Top
         tft.fillRect(0, 107, 107, 2, 0x00FB); // Bottom
         tft.fillRect(107, 40, 2, 69, 0x00FB); // Right
         tft.fillRect(0, 40, 2, 67, 0x00FB); // Left

         drawRGBControlSubScreen(true, true, 3);
       }
       else if(SectionMode[0] == 2 &&
               SectionMode[1] == 2 &&
               SectionMode[2] == 2 &&
               SectionMode[3] == 2 &&
               SectionMode[4] == 2 &&
               SectionMode[5] == 2) { // All on Disco
         // Highlight Disco
         tft.fillRect(0, 174, 107, 2, 0x00FB); // Top
         tft.fillRect(0, 238, 107, 2, 0x00FB); // Bottom
         tft.fillRect(107, 174, 2, 69, 0x00FB); // Right
         tft.fillRect(0, 174, 2, 67, 0x00FB); // Left

        drawDiscoControlSubScreen(true, true, 3);
       }
       else {
         drawAllControlSubScreen();
       }
    }
    else {
      if(SectionMode[ControlScreenSectionNum-1] == 0) { // Individual on Cycle
        // Highlight Cycle
        tft.fillRect(0, 107, 107, 2, 0x00FB); // Top
        tft.fillRect(0, 174, 107, 2, 0x00FB); // Bottom
        tft.fillRect(107, 107, 2, 69, 0x00FB); // Right
        tft.fillRect(0, 107, 2, 67, 0x00FB); // Left

        drawCycleControlSubScreen(true, true, 2);
      }
      else if(SectionMode[ControlScreenSectionNum-1] == 1) { // Indiv. on RGB
        // Highlight RGB
        tft.fillRect(0, 40, 107, 2, 0x00FB); // Top
        tft.fillRect(0, 107, 107, 2, 0x00FB); // Bottom
        tft.fillRect(107, 40, 2, 69, 0x00FB); // Right
        tft.fillRect(0, 40, 2, 67, 0x00FB); // Left

        drawRGBControlSubScreen(true, true, 3);
      }
      else if(SectionMode[ControlScreenSectionNum-1] == 2) { // Indiv. on Disco
        // Highlight Disco
        tft.fillRect(0, 174, 107, 2, 0x00FB); // Top
        tft.fillRect(0, 238, 107, 2, 0x00FB); // Bottom
        tft.fillRect(107, 174, 2, 69, 0x00FB); // Right
        tft.fillRect(0, 174, 2, 67, 0x00FB); // Left

        drawDiscoControlSubScreen(true, true, 3);
      }
    }

    // Buttons
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextColor(0x00FB);

    tft.setCursor(15, 85);
    tft.print("RGB");

    tft.setCursor(8, 150);
    tft.print("Cycle");

    tft.setCursor(6, 220);
    tft.print("Disco");
  }

  else {
    // Back Button (Feedback)
    tft.fillTriangle(6, 20, 26, 6, 26, 33, 0x00FB);
    tft.fillTriangle(15, 20, 26, 12, 26, 27, 0x0000);

    // Dividers
    tft.fillRect(0, 40, 107, 2, 0x0000);
    tft.fillRect(0, 107, 107, 2, 0x0000);
    tft.fillRect(0, 174, 107, 2, 0x0000);
    tft.fillRect(0, 238, 107, 2, 0x0000);
    tft.fillRect(107, 40, 2, 200, 0x0000);
    tft.fillRect(0, 40, 2, 200, 0x0000);

    // Buttons
    tft.fillRect(2, 42, 103, 63, 0x0000);
    tft.fillRect(2, 109, 103, 63, 0x0000);
    tft.fillRect(2, 176, 103, 63, 0x0000);

    // Screen Title
    tft.setCursor(32, 32);
    tft.fillRect(0, 0, 320, 40, 0x0000);
  }
}


void controlScreenTouchCheck() {
  // Waits for screen to be tapped
  TSPoint p = ts.getPoint();
  if(p.z > ts.pressureThreshhold) {

    if((p.x < 270) && (p.y > 630)) { // Back Arrow
      clearControlSubScreen();
      drawControlScreen(false);

      // Go to the main screen
      CurrScreen = 0;
      drawMainScreen(true);
    }

    else if((p.x > 270) && (p.x < 490) && (p.y > 630)) { // RGB
      // Deselect Others on Tap
      tft.fillRect(0, 40, 107, 2, 0xF974);
      tft.fillRect(0, 107, 107, 2, 0xF974);
      tft.fillRect(0, 174, 107, 2, 0xF974);
      tft.fillRect(0, 238, 107, 2, 0xF974);
      tft.fillRect(107, 40, 2, 200, 0xF974);
      tft.fillRect(0, 40, 2, 200, 0xF974);

      // Highlight on Tap
      tft.fillRect(0, 40, 107, 2, 0x00FB); // Top
      tft.fillRect(0, 107, 107, 2, 0x00FB); // Bottom
      tft.fillRect(107, 40, 2, 69, 0x00FB); // Right
      tft.fillRect(0, 40, 2, 67, 0x00FB); // Left

      if(ControlScreenSectionNum == 0) {
        for(int i = 0; i < 6; i++) {
          SectionMode[i] = 1; // RGB
        }

        // Set the RGB value for All to the average of all
        AllRGB[0] = (SectionRGB[0][0] + SectionRGB[1][0] + SectionRGB[2][0] +
                     SectionRGB[3][0] + SectionRGB[4][0] + SectionRGB[5][0])/6;
        AllRGB[1] = (SectionRGB[0][1] + SectionRGB[1][1] + SectionRGB[2][1] +
                     SectionRGB[3][1] + SectionRGB[4][1] + SectionRGB[5][1])/6;
        AllRGB[1] = (SectionRGB[0][2] + SectionRGB[1][2] + SectionRGB[2][2] +
                     SectionRGB[3][2] + SectionRGB[4][2] + SectionRGB[5][2])/6;
      }
      else {
        SectionMode[ControlScreenSectionNum-1] = 1; // RGB
      }

      clearControlSubScreen(); // Clear screen before drawing sliders

      drawRGBControlSubScreen(true, true, 3);

    }
    else if((p.x > 490) && (p.x < 690) && (p.y > 630)) { // Cycle
      // Deselect Others on Tap
      tft.fillRect(0, 40, 107, 2, 0xF974);
      tft.fillRect(0, 107, 107, 2, 0xF974);
      tft.fillRect(0, 174, 107, 2, 0xF974);
      tft.fillRect(0, 238, 107, 2, 0xF974);
      tft.fillRect(107, 40, 2, 200, 0xF974);
      tft.fillRect(0, 40, 2, 200, 0xF974);

      // Highlight on Tap
      tft.fillRect(0, 107, 107, 2, 0x00FB); // Top
      tft.fillRect(0, 174, 107, 2, 0x00FB); // Bottom
      tft.fillRect(107, 107, 2, 69, 0x00FB); // Right
      tft.fillRect(0, 107, 2, 67, 0x00FB); // Left

      if(ControlScreenSectionNum == 0) {
        for(int i = 0; i < 6; i++) {
          SectionMode[i] = 0; // Cycle
        }
      }
      else {
        SectionMode[ControlScreenSectionNum-1] = 0; // Cycle
      }

      clearControlSubScreen(); // Clear screen before drawing sliders

      drawCycleControlSubScreen(true, true, 2);
    }
    else if((p.x > 690) && (p.y > 630)) { // Disco
      // Deselect Others on Tap
      tft.fillRect(0, 40, 107, 2, 0xF974);
      tft.fillRect(0, 107, 107, 2, 0xF974);
      tft.fillRect(0, 174, 107, 2, 0xF974);
      tft.fillRect(0, 238, 107, 2, 0xF974);
      tft.fillRect(107, 40, 2, 200, 0xF974);
      tft.fillRect(0, 40, 2, 200, 0xF974);

      // Highlight on Tap
      tft.fillRect(0, 174, 107, 2, 0x00FB); // Top
      tft.fillRect(0, 238, 107, 2, 0x00FB); // Bottom
      tft.fillRect(107, 174, 2, 69, 0x00FB); // Right
      tft.fillRect(0, 174, 2, 67, 0x00FB); // Left

      if(ControlScreenSectionNum == 0) {
        for(int i = 0; i < 6; i++) {
          SectionMode[i] = 2; // Disco
        }
      }
      else {
        SectionMode[ControlScreenSectionNum-1] = 2; // Disco
      }

      clearControlSubScreen(); // Clear screen before drawing sliders

      drawDiscoControlSubScreen(true, true, 3);
    }
  }

  // Check for touch on subscreens

  // Deal with touches on the "ALL" screen
  if(ControlScreenSectionNum == 0) {
    // Check for touch if on Cycle Mode
    if(SectionMode[0] == 0 &&
       SectionMode[1] == 0 &&
       SectionMode[2] == 0 &&
       SectionMode[3] == 0 &&
       SectionMode[4] == 0 &&
       SectionMode[5] == 0) { // All on cycle
      cycleControlSubScreenTouchCheck();
    }
    // Check for touch if on RGB Mode
    else if(SectionMode[0] == 1 &&
            SectionMode[1] == 1 &&
            SectionMode[2] == 1 &&
            SectionMode[3] == 1 &&
            SectionMode[4] == 1 &&
            SectionMode[5] == 1) { // All on RGB
      RGBControlSubScreenTouchCheck();
    }
    // Check for touch if on Disco Mode
    else if(SectionMode[0] == 2 &&
            SectionMode[1] == 2 &&
            SectionMode[2] == 2 &&
            SectionMode[3] == 2 &&
            SectionMode[4] == 2 &&
            SectionMode[5] == 2) { // All on Disco
      discoControlSubScreenTouchCheck();
    }
  }
  else {
    // Check for touch if on Cycle Mode
    if(SectionMode[ControlScreenSectionNum-1] == 0) {
      cycleControlSubScreenTouchCheck();
    }
    // Check for touch if on RGB Mode
    else if(SectionMode[ControlScreenSectionNum-1] == 1) {
      RGBControlSubScreenTouchCheck();
    }
    // Check for touch if on Disco Mode
    else if(SectionMode[ControlScreenSectionNum-1] == 2) {
      discoControlSubScreenTouchCheck();
    }
  }
}


void drawRGBControlSubScreen(bool drawMode, bool sliders, int sliderColor) {
  // Red Slider
  if(sliderColor == 0) {
    // Draw Sliders
    if(sliders) {
      tft.drawRoundRect(117, 52, 193, 51, 15, 0xF805);  // Red
    }

    // Draw Controllers
    if(ControlScreenSectionNum == 0) {
      if(drawMode) {
        tft.fillRoundRect(118 + (AllRGB[0] * 0.56), 53, 49, 49, 14, 0xFB0C);  // Red
      }
      else{
        tft.fillRoundRect(118 + (AllRGB[0] * 0.56), 53, 49, 49, 14, 0x0000);  // Red
      }
    }
    else {
      if(drawMode) {
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][0] * 0.56), 53, 49, 49, 14, 0xFB0C);  // Red
      }
      else{
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][0] * 0.56), 53, 49, 49, 14, 0x0000);  // Red
      }
    }
  }

  // Green Slider
  else if(sliderColor == 1) {
    // Draw Sliders
    if(sliders) {
      tft.drawRoundRect(117, 114, 193, 51, 15, 0x07EE); // Green
    }

    // Draw Controllers
    if(ControlScreenSectionNum == 0) {
      if(drawMode) {
        tft.fillRoundRect(118 + (AllRGB[1] * 0.56), 115, 49, 49, 14, 0x6FF5); // Green
      }
      else{
        tft.fillRoundRect(118 + (AllRGB[1] * 0.56), 115, 49, 49, 14, 0x0000); // Green
      }
    }
    else {
      if(drawMode) {
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][1] * 0.56), 115, 49, 49, 14, 0x6FF5); // Green
      }
      else{
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][1] * 0.56), 115, 49, 49, 14, 0x0000); // Green
      }
    }
  }

  // Blue Slider
  else if(sliderColor == 2) {
    // Draw Sliders
    if(sliders) {
      tft.drawRoundRect(117, 176, 193, 51, 15, 0x029F); // Blue
    }

    // Draw Controllers
    if(ControlScreenSectionNum == 0) {
      if(drawMode) {
        tft.fillRoundRect(118 + (AllRGB[2] * 0.56), 177, 49, 49, 14, 0x64BF); // Blue
      }
      else{
        tft.fillRoundRect(118 + (AllRGB[2] * 0.56), 177, 49, 49, 14, 0x0000); // Blue
      }
    }
    else {
      if(drawMode) {
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][2] * 0.56), 177, 49, 49, 14, 0x64BF); // Blue
      }
      else{
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][2] * 0.56), 177, 49, 49, 14, 0x0000); // Blue
      }
    }
  }

  // All Sliders
  else if(sliderColor == 3) {
    // Draw Sliders
    if(sliders) {
      tft.drawRoundRect(117, 52, 193, 51, 15, 0xF805);  // Red
      tft.drawRoundRect(117, 114, 193, 51, 15, 0x07EE); // Green
      tft.drawRoundRect(117, 176, 193, 51, 15, 0x029F); // Blue
    }

    // Draw Controllers
    if(ControlScreenSectionNum == 0) {
      if(drawMode) {
        tft.fillRoundRect(118 + (AllRGB[0] * 0.56), 53, 49, 49, 14, 0xFB0C);  // Red
        tft.fillRoundRect(118 + (AllRGB[1] * 0.56), 115, 49, 49, 14, 0x6FF5); // Green
        tft.fillRoundRect(118 + (AllRGB[2] * 0.56), 177, 49, 49, 14, 0x64BF); // Blue
      }
      else{
        tft.fillRoundRect(118 + (AllRGB[0] * 0.56), 53, 49, 49, 14, 0x0000);  // Red
        tft.fillRoundRect(118 + (AllRGB[1] * 0.56), 115, 49, 49, 14, 0x0000); // Green
        tft.fillRoundRect(118 + (AllRGB[2] * 0.56), 177, 49, 49, 14, 0x0000); // Blue
      }
    }
    else {
      if(drawMode) {
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][0] * 0.56), 53, 49, 49, 14, 0xFB0C);  // Red
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][1] * 0.56), 115, 49, 49, 14, 0x6FF5); // Green
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][2] * 0.56), 177, 49, 49, 14, 0x64BF); // Blue
      }
      else{
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][0] * 0.56), 53, 49, 49, 14, 0x0000);  // Red
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][1] * 0.56), 115, 49, 49, 14, 0x0000); // Green
        tft.fillRoundRect(118 + (SectionRGB[ControlScreenSectionNum-1][2] * 0.56), 177, 49, 49, 14, 0x0000); // Blue
      }
    }
  }
}


void RGBControlSubScreenTouchCheck() {
  // Wait for screen to be tapped
  TSPoint p = ts.getPoint();

  // Control RGB if NOT on the "All" screen
  if(p.z > ts.pressureThreshhold && ControlScreenSectionNum != 0) {
    if((p.x > 290) && (p.x < 475) && (p.y < 540) && (p.y > 95)) { // Red
      drawRGBControlSubScreen(false, false, 0);

      // Change the red value based on where the screen was tapped
      SectionRGB[ControlScreenSectionNum-1][0] = ceil((-0.653846)*(p.y) + 353.0768);

      // Round it if it's near the top or bottom to make it look pretty
      if(SectionRGB[ControlScreenSectionNum-1][0] < 10) {
        SectionRGB[ControlScreenSectionNum-1][0] = 0;
      }
      else if(SectionRGB[ControlScreenSectionNum-1][0] > 245) {
        SectionRGB[ControlScreenSectionNum-1][0] = 255;
      }

      drawRGBControlSubScreen(true, false, 0);
    }
    else if((p.x > 475) && (p.x < 685) && (p.y < 600) && (p.y > 95)) { // Green
      drawRGBControlSubScreen(false, false, 1);

      // Change the green value based on where the screen was tapped
      SectionRGB[ControlScreenSectionNum-1][1] = ceil((-0.653846)*(p.y) + 353.0768);

      // Round it if it's near the top or bottom to make it look pretty
      if(SectionRGB[ControlScreenSectionNum-1][1] < 10) {
        SectionRGB[ControlScreenSectionNum-1][1] = 0;
      }
      else if(SectionRGB[ControlScreenSectionNum-1][1] > 245) {
        SectionRGB[ControlScreenSectionNum-1][1] = 255;
      }

      drawRGBControlSubScreen(true, false, 1);
    }
    else if((p.x > 685) && (p.y < 600) && (p.y > 95)) { // Blue
      drawRGBControlSubScreen(false, false, 2);

      // Change the blue value based on where the screen was tapped
      SectionRGB[ControlScreenSectionNum-1][2] = ceil((-0.653846)*(p.y) + 353.0768);

      // Round it if it's near the top or bottom to make it look pretty
      if(SectionRGB[ControlScreenSectionNum-1][2] < 10) {
        SectionRGB[ControlScreenSectionNum-1][2] = 0;
      }
      else if(SectionRGB[ControlScreenSectionNum-1][2] > 245) {
        SectionRGB[ControlScreenSectionNum-1][2] = 255;
      }

      drawRGBControlSubScreen(true, false, 2);
    }
  }

  // Control RGB if on the "All" screen
  else if(p.z > ts.pressureThreshhold && ControlScreenSectionNum == 0) {
    if((p.x > 290) && (p.x < 475) && (p.y < 540) && (p.y > 95)) { // Red
      drawRGBControlSubScreen(false, false, 0);

      // Change the red value based on where the screen was tapped
      AllRGB[0] = ceil((-0.653846)*(p.y) + 353.0768); // Set all to keep track

      // Round it if it's near the top or bottom to make it look pretty
      if(AllRGB[0] < 10) {
        AllRGB[0] = 0;
      }
      else if(AllRGB[0] > 245) {
        AllRGB[0] = 255;
      }

      // Change all the sections' red value
      for(int i = 0; i < 6; i++) {
        SectionRGB[i][0] = AllRGB[0];
      }

      drawRGBControlSubScreen(true, false, 0);
    }
    else if((p.x > 475) && (p.x < 685) && (p.y < 600) && (p.y > 95)) { // Green
      drawRGBControlSubScreen(false, false, 1);

      // Change the green value based on where the screen was tapped
      AllRGB[1] = ceil((-0.653846)*(p.y) + 353.0768);

      // Round it if it's near the top or bottom to make it look pretty
      if(AllRGB[1] < 10) {
        AllRGB[1] = 0;
      }
      else if(AllRGB[1] > 245) {
        AllRGB[1] = 255;
      }

      // Change all the sections' green value
      for(int i = 0; i < 6; i++) {
        SectionRGB[i][1] = AllRGB[1];
      }

      drawRGBControlSubScreen(true, false, 1);
    }
    else if((p.x > 685) && (p.y < 600) && (p.y > 95)) { // Blue
      drawRGBControlSubScreen(false, false, 2);

      // Change the blue value based on where the screen was tapped
      AllRGB[2] = ceil((-0.653846)*(p.y) + 353.0768);

      // Round it if it's near the top or bottom to make it look pretty
      if(AllRGB[2] < 10) {
        AllRGB[2] = 0;
      }
      else if(AllRGB[2] > 245) {
        AllRGB[2] = 255;
      }

      // Change all the sections' blue value
      for(int i = 0; i < 6; i++) {
        SectionRGB[i][2] = AllRGB[2];
      }

      drawRGBControlSubScreen(true, false, 2);
    }
  }
}


void drawCycleControlSubScreen(bool drawMode, bool sliders, int sliderNumber) {
  // Draw Sliders
  if(sliders) {
    // Slider Names
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(0xF974);

    tft.setCursor(122, 74);
    tft.print("Fade Speed");
    tft.setCursor(122, 169);
    tft.print("Brightness");

    tft.drawRoundRect(117, 84, 193, 51, 15, 0xFFFF);  // Fade Speed
    tft.drawRoundRect(117, 179, 193, 51, 15, 0xFFFF); // Brightness
  }

  // Draw Controllers
  if(sliderNumber == 0) { // Fade Speed
    if(drawMode) {
      tft.fillRoundRect(118 + (FadeSpeed * 2.84), 85, 49, 49, 14, 0x7BEF);  // Fade Speed
    }
    else{
      tft.fillRoundRect(118 + (FadeSpeed * 2.84), 85, 49, 49, 14, 0x0000);  // Fade Speed
    }
  }
  else if(sliderNumber == 1) { // Brightness
    if(drawMode) {
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x7BEF); // Brightness
    }
    else{
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x0000); // Brightness
    }
  }
  else if(sliderNumber == 2) { // All
    if(drawMode) {
      tft.fillRoundRect(118 + (FadeSpeed * 2.84), 85, 49, 49, 14, 0x7BEF);  // Fade Speed
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x7BEF); // Brightness
    }
    else{
      tft.fillRoundRect(118 + (FadeSpeed * 2.84), 85, 49, 49, 14, 0x0000);  // Fade Speed
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x0000); // Brightness
    }
  }
}


void cycleControlSubScreenTouchCheck() {
  // Wait for screen to be tapped
  TSPoint p = ts.getPoint();

  if(p.z > ts.pressureThreshhold) {
    if((p.x > 400) && (p.x < 650) && (p.y < 540) && (p.y > 150)) { // Fade Speed
      drawCycleControlSubScreen(false, false, 0);

      // Change the fade speed value based on where the screen was tapped
      // FadeSpeed = ceil((-0.25385)*(p.y) + 137.0769);
      FadeSpeed = ceil((-0.10889)*(p.y) + 66.34);


      // Round it if it's near the top or bottom to make it look pretty
      if(FadeSpeed < 10) {
        FadeSpeed = 1;
      }
      else if(FadeSpeed > 40) {
        FadeSpeed = 50;
      }

      drawCycleControlSubScreen(true, false, 0);
    }
    else if((p.x > 650) && (p.y < 600) && (p.y > 95)) { // Brightness
      drawCycleControlSubScreen(false, false, 1);

      // Change the brightness value based on where the screen was tapped
      Brightness = ceil((-0.25385)*(p.y) + 137.0769);

      // Round it if it's near the top or bottom to make it look pretty
      if(Brightness < 10) {
        Brightness = 0;
      }
      else if(Brightness > 90) {
        Brightness = 100;
      }

      drawCycleControlSubScreen(true, false, 1);
    }
  }
}


void drawDiscoControlSubScreen(bool drawMode, bool sliders, int sliderNumber) {
  // Draw Sliders
  if(sliders) {
    // Slider Names
    tft.setFont(&FreeSans9pt7b);
    tft.setTextColor(0xF974);

    tft.setCursor(212, 53);
    tft.print("Sync");
    tft.setCursor(122, 98);
    tft.print("Flash Speed");
    tft.setCursor(122, 172);
    tft.print("Brightness");

    tft.drawRoundRect(259, 40, 51, 51, 15, 0xFFFF); // Sync
    tft.drawRoundRect(117, 105, 193, 51, 15, 0xFFFF); // Flash Speed
    tft.drawRoundRect(117, 179, 193, 51, 15, 0xFFFF); // Brightness
  }

  // Draw Controllers
  if(sliderNumber == 0) { // Flash Speed
    if(drawMode) {
      tft.fillRoundRect(118 + (FlashSpeed * -0.0364 + 145.641), 106, 49, 49, 14, 0x7BEF);  // Flash Speed
    }
    else{
      tft.fillRoundRect(118 + (FlashSpeed * -0.0364 + 145.641), 106, 49, 49, 14, 0x0000);  // Flash Speed
    }
  }
  else if(sliderNumber == 1) { // Brightness
    if(drawMode) {
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x7BEF); // Brightness
    }
    else{
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x0000); // Brightness
    }
  }
  else if(sliderNumber == 2) { // Sync
    if(drawMode) {
      if(DiscoSync) {
        tft.fillRoundRect(260, 41, 49, 49, 14, 0x7BEF); // Sync
      }
    }
    else {
      tft.fillRoundRect(260, 41, 49, 49, 14, 0x0000); // Sync
    }
  }
  else if(sliderNumber == 3) { // All
    if(drawMode) {
      tft.fillRoundRect(118 + (FlashSpeed * -0.0364 + 145.641), 106, 49, 49, 14, 0x7BEF);  // Flash Speed
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x7BEF); // Brightness
      if(DiscoSync) {
        tft.fillRoundRect(260, 41, 49, 49, 14, 0x7BEF); // Sync
      }
    }
    else{
      tft.fillRoundRect(118 + (FlashSpeed * -0.0364 + 145.641), 106, 49, 49, 14, 0x0000);  // Flash Speed
      tft.fillRoundRect(118 + (Brightness * 1.42), 180, 49, 49, 14, 0x0000); // Brightness
      tft.fillRoundRect(260, 41, 49, 49, 14, 0x0000); // Sync
    }
  }
}


void discoControlSubScreenTouchCheck() {
  // Wait for screen to be tapped
  TSPoint p = ts.getPoint();

  if(p.z > ts.pressureThreshhold) {
    if((p.x > 240) && (p.x < 450) && (p.y < 270) && (p.y > 95)) {
      if(DiscoSync && ScreenTouched <= 0) {
        DiscoSync = false;
        drawDiscoControlSubScreen(false, false, 2);
      }
      else if(ScreenTouched <= 0) {
        DiscoSync = true;
        drawDiscoControlSubScreen(true, false, 2);
      }
      ScreenTouched = 10; // So the toggle switch doesn't flicker
    }

    else if((p.x > 450) && (p.x < 680) && (p.y < 600) && (p.y > 95)) { // Flash Speed
      drawDiscoControlSubScreen(false, false, 0);

      // Change the brightness value based on where the screen was tapped
      FlashSpeed = ceil((10)*(p.y) - 1400);

      // Round it if it's near the top or bottom to make it look pretty
      if(FlashSpeed < 200) {
        FlashSpeed = 100;
      }
      else if(FlashSpeed > 3900) {
        FlashSpeed = 4000;
      }

      drawDiscoControlSubScreen(true, false, 0);
    }
    else if((p.x > 680) && (p.y < 600) && (p.y > 95)) { // Brightness
      drawDiscoControlSubScreen(false, false, 1);

      // Change the brightness value based on where the screen was tapped
      Brightness = ceil((-0.25385)*(p.y) + 137.0769);

      // Round it if it's near the top or bottom to make it look pretty
      if(Brightness < 10) {
        Brightness = 0;
      }
      else if(Brightness > 90) {
        Brightness = 100;
      }

      drawDiscoControlSubScreen(true, false, 1);
    }
  }

  // Decrement the ScreenTouched any time the screen isn't being touched
  if(ScreenTouched > 0) {
    ScreenTouched--;
  }
}


void drawAllControlSubScreen() {
  /* Shows the current state of the lights if at least one of the lights
   * is in a different state.
   */

  // Draw Rectangles
  tft.drawRoundRect(117, 42, 94, 61, 15, 0xF974);  // Dash
  tft.drawRoundRect(117, 109, 94, 61, 15, 0xF974); // DF
  tft.drawRoundRect(117, 176, 94, 61, 15, 0xF974); // DR

  tft.drawRoundRect(217, 42, 94, 61, 15, 0xF974);  // HVAC
  tft.drawRoundRect(217, 109, 94, 61, 15, 0xF974); // PF
  tft.drawRoundRect(217, 176, 94, 61, 15, 0xF974); // PR

  // Draw Section Names
  tft.setFont(&FreeSans9pt7b);
  tft.setTextColor(0x7BEF);

  tft.setCursor(144, 69);
  tft.print("Dash");
  tft.setCursor(154, 136);
  tft.print("DF");
  tft.setCursor(152, 203);
  tft.print("DR");
  tft.setCursor(240, 69);
  tft.print("HVAC");
  tft.setCursor(253, 136);
  tft.print("PF");
  tft.setCursor(252, 203);
  tft.print("PR");

  // Draw The Mode Each Section is in
  tft.setTextColor(0x00FB);

  for(int i = 0; i < 6; i++) {
    // Draw all cycle modes
    if(SectionMode[i] == 0) {
      if(i%2 == 0) { // Left side
        tft.setCursor(142, 85+(i*34));
        tft.print("Cycle");
      }
      else { // Right side
        tft.setCursor(242, 85+((i-1)*34));
        tft.print("Cycle");
      }
    }

    else if(SectionMode[i] == 1) {
      if(i%2 == 0) { // Left side
        tft.setCursor(144, 85+(i*34));
        tft.print("RGB");
      }
      else { // Right side
        tft.setCursor(244, 85+((i-1)*34));
        tft.print("RGB");
      }
    }

    else if(SectionMode[i] == 2) {
      if(i%2 == 0) { // Left side
        tft.setCursor(142, 85+(i*34));
        tft.print("Disco");
      }
      else { // Right side
        tft.setCursor(242, 85+((i-1)*34));
        tft.print("Disco");
      }
    }
  }
}


void clearControlSubScreen() {
  tft.fillRect(109, 40, 211, 200, 0x0000);
}


// Wave Screen
void drawWaveScreen(bool drawMode) {
  if(drawMode) {
    // Back Button
    tft.fillTriangle(6, 20, 26, 6, 26, 33, 0xF974);
    tft.fillTriangle(15, 20, 26, 12, 26, 27, 0x0000);

    // Screen Title
    tft.setCursor(32, 32);
    tft.setFont(&FreeSansBold18pt7b);
    tft.setTextColor(0x00FB);
    tft.print("Waves");
  }
  else {
    // Back Button (Feedback)
    tft.fillTriangle(6, 20, 26, 6, 26, 33, 0x00FB);
    tft.fillTriangle(15, 20, 26, 12, 26, 27, 0x0000);

    // Screen Title
    tft.setCursor(32, 32);
    tft.fillRect(0, 0, 320, 40, 0x0000);
  }
}


void waveCheck() {
  // Waits for screen to be tapped
  TSPoint p = ts.getPoint();
  if(p.z > ts.pressureThreshhold) {

    if((p.x < 270) && (p.y > 630)) { // Back Arrow
      drawWaveScreen(false);

      // Go to the main screen
      CurrScreen = 0;
      drawMainScreen(true);
    }
  }
}


// Other
void updateLights() {

  // Set the sections in "cycle" mode to the most recent fade color value
  for(int i = 0; i < 6; i++) {
    if(SectionMode[i] == 0) {
      generateRBG(i, FadeColor);
    }
  }

  // Set the sections in "RGB" mode to their color value
  for(int i = 0; i < 6; i++) {
    if(SectionMode[i] == 1) {
      showColor(SectionRGB[i][0], SectionRGB[i][1], SectionRGB[i][2], i);
    }
  }

  // Set the sections in "Disco" mode to their color value
  int currMillis = millis();

  if((currMillis - LastDiscoMillis) > FlashSpeed) {
    LastDiscoMillis = currMillis;

    if(DiscoSync) {
      int discoColor = random(0, 1531);

      for(int i = 0; i < 6; i++) {
        if(SectionMode[i] == 2) {
          generateRBG(i, discoColor);
        }
      }
    }
    else {
      for(int i = 0; i < 6; i++) {
        if(SectionMode[i] == 2) {
          generateRBG(i, random(0, 1531));
        }
      }
    }
  }

  // Update the state of the fade
  FadeColor += FadeSpeed;
  if(FadeColor >= 1530) {
    FadeColor -= 1530;
  }
}


void showColor(int red, int green, int blue, int section) {

  // Set the brightness
  red = ceil(red * (float(Brightness)/100));
  green = ceil(green * (float(Brightness)/100));
  blue = ceil(blue * (float(Brightness)/100));

  if(section == 0) {
    // Dash
    for(int i = 18; i < 25; i++) {
      leds[i].setRGB(red, green, blue);
    }
    FastLED.show();
  }

  else if(section == 1) {
    // Climate Controls
    analogWrite(LED_RED, red);
    analogWrite(LED_GREEN, green);
    analogWrite(LED_BLUE, blue);
  }

  else if(section == 2) {
    // Driver Front
    for(int i = 5; i < 10; i++) {
      leds[i].setRGB(red, green, blue);
    }
    FastLED.show();
  }

  else if(section == 3) {
    // Passenger Front
    for(int i = 0; i < 5; i++) {
      leds[i].setRGB(red, green, blue);
    }
    FastLED.show();
  }

  else if(section == 4) {
    // Driver Rear
    for(int i = 14; i < 18; i++) {
      leds[i].setRGB(red, green, blue);
    }
    FastLED.show();
  }

  else if(section == 5) {
    // Passenger Rear
    for(int i = 10; i < 14; i++) {
      leds[i].setRGB(red, green, blue);
    }
    FastLED.show();
  }
}


void generateRBG(int section, int colorNumber) {
  if(colorNumber >= 0 && colorNumber <= 255) {
    SectionRGB[section][0] = 255;
    SectionRGB[section][1] = 0;
    SectionRGB[section][2] = colorNumber;
    // Check to see if "SectionRGB[section] = {255, 0, colorNumber};" works

    showColor(255, 0, colorNumber, section);
  }

  else if(colorNumber > 255 && colorNumber <= 510) {
    SectionRGB[section][0] = 510-colorNumber;
    SectionRGB[section][1] = 0;
    SectionRGB[section][2] = 255;

    showColor(510-colorNumber, 0, 255, section);
  }

  else if(colorNumber > 510 && colorNumber <= 765) {
    SectionRGB[section][0] = 0;
    SectionRGB[section][1] = colorNumber-510;
    SectionRGB[section][2] = 255;

    showColor(0, colorNumber-510, 255, section);
  }

  else if(colorNumber > 765 && colorNumber <= 1020) {
    SectionRGB[section][0] = 0;
    SectionRGB[section][1] = 255;
    SectionRGB[section][2] = 1020-colorNumber;

    showColor(0, 255, 1020-colorNumber, section);
  }

  else if(colorNumber > 1020 && colorNumber <= 1275) {
    SectionRGB[section][0] = colorNumber-1020;
    SectionRGB[section][1] = 255;
    SectionRGB[section][2] = 0;

    showColor(colorNumber-1020, 255, 0, section);
  }

  else if(colorNumber > 1275 && colorNumber <= 1530) {
    SectionRGB[section][0] = 255;
    SectionRGB[section][1] = 1530-colorNumber;
    SectionRGB[section][2] = 0;

    showColor(255, 1530-colorNumber, 0, section);
  }
}


// Main
void loop() {
  // Fade in on Startup
  if(StartupFadeIn) {
    Brightness++;
  }
  if(Brightness >= 100) {
    StartupFadeIn = false;
  }


  if(CurrScreen == 0) {
    mainScreenTouchCheck();
  }
  else if(CurrScreen == 1) {
    hideTouchCheck();
  }
  else if(CurrScreen == 2) {
    controlScreenTouchCheck();
  }
  else if(CurrScreen == 3) {
    waveCheck();
  }

  updateLights();
}
