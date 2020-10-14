// Weather sensor program for TTGO T-Display w/onboard TFT + BME280 sensor
#define FS_NO_GLOBALS
#define SPWIDTH  240                 // sprite width
#define SPHEIGHT 135                 // sprite height

#include <Adafruit_BME280.h>         // Include the BME280 library (works with 2.0.2+)
#include <TFT_eSPI.h>                // Include the graphics library
#include <U8g2_for_TFT_eSPI.h>       // Include the u8g2 font renderer
#include <EasyButton.h>              // Include the button handler
#include "Free_Fonts.h"              // Reference to the free fonts header

//Global variables for displayed values
float Pa;                            
float Baro;
float TempC;
float TempF;
float TempHi;
float TempLo;
float Humid;
float HumidHi;
float HumidLo;
float SLPressure;
int Elevation = 549;                  // Elevation of current location in Meters
int displayMode = 1;
char cTempF[10];
char cTempHi[10];
char cTempLo[10];
char cBaro[10];
char cHumid[10];
char cHumidHi[10];
char cHumidLo[10];
long lastUpdate = 0;

// Initialize devices
Adafruit_BME280 bme;                  // BME sensor instance
TFT_eSPI tft = TFT_eSPI();            // tft instance
U8g2_for_TFT_eSPI u8g2;               // u8g2 font instance
U8g2_for_TFT_eSPI u8g2x;              
TFT_eSprite spr = TFT_eSprite(&tft);  // configure sprites for flicker free updates
TFT_eSprite spr1 = TFT_eSprite(&tft);
EasyButton button1(0);                // setup button on pin 0

void setup() {

  tft.begin();                        // Enable the display
  tft.setRotation(3);                 // set display rotation to landscape
  u8g2.begin(spr);                    // Enable the Font renderer
  u8g2x.begin(spr1);
  bme.begin(0x76, &Wire);             // Enable the bme280
 
  if (!SPIFFS.begin()) {              // init SPIFFS
    while (1) yield();                // Stay here twiddling thumbs waiting for SPIFFS
  }
  button1.begin();
  button1.onPressed(onPress1);
  //initial values for hi/lo 
  TempHi = 0;
  TempLo = 150;
  HumidHi = 0;
  HumidLo = 100;
  tft.fillScreen(TFT_BLACK);          // clear the screen
  drawScreen();                       // initial draw of display

}
//end setup



void readSensors() {

  TempC = bme.readTemperature();      // read temp in Celsius
  Pa = bme.readPressure();            // read the pressure in Pascals
  Humid = bme.readHumidity();         // read humidity in percent
  TempF = ((TempC * 1.8) + 32.0);     // calculate temp in Farenheit
  SLPressure = (Pa) / pow((1 - ((float)(Elevation)) / 44330), 5.255); // Calculates sea level reference pressure from measured value and Elevation
  Baro = SLPressure / 3386.39;        // corrected barometric pressure
  // store hi and lo values
  if (Humid > HumidHi) HumidHi = Humid;
  if (Humid < HumidLo) HumidLo = Humid; 
  if (TempF > TempHi) TempHi = TempF;
  if (TempF < TempLo) TempLo = TempF; 
  // convert float to string
  dtostrf(TempF, 2, 0, cTempF);
  dtostrf(TempHi, 2, 0, cTempHi);
  dtostrf(TempLo, 2, 0, cTempLo);
  dtostrf(Baro, 4, 2, cBaro);
  dtostrf(Humid, 2, 0, cHumid);
  dtostrf(HumidHi, 2, 0, cHumidHi);
  dtostrf(HumidLo, 2, 0, cHumidLo);    
}

// end readSensors

void onPress1() {                     // do something when the button is pressed
  displayMode ++;                     // increment the display mode
  drawScreen();                       // draw the screen
}
//end onPress1

void writeTemp() {

  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_BLACK);
  u8g2.setForegroundColor(TFT_RED);
  u8g2.setFont(u8g2_font_helvR14_tf);
  u8g2.setFontMode(1);
  u8g2.setCursor(0, 20);
  u8g2.print("Temperature (°F)");
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Temperature")/2), 20, "Temperature");
  u8g2.setCursor(0, 73);
  u8g2.print("Hi:");
  u8g2.setCursor(0, 113);
  u8g2.print("Lo:");
  spr.drawFastHLine(0, 30, 239, TFT_CYAN);
  spr.drawFastVLine(95, 30, 105, TFT_CYAN);
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontSmall);
  spr.drawRightString(cTempHi, 85, 55, GFXFF);
  spr.drawRightString(cTempLo, 85, 95, GFXFF);
  spr.unloadFont();
  spr.loadFont(fontLarge);
  spr.drawRightString(cTempF, 239, 60, GFXFF);
  spr.unloadFont();
  spr.pushSprite(0, 0);
  spr.deleteSprite();

}

void writeHumid() {

  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_BLACK);
  u8g2.setForegroundColor(TFT_RED);
  u8g2.setFont(u8g2_font_helvR14_tr);
  u8g2.setFontMode(1);
  u8g2.setCursor(0, 20);
  u8g2.print("Humidity (%)");
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Humidity")/2), 20, "Humidity");
  u8g2.setCursor(0, 73);
  u8g2.print("Hi:");
  u8g2.setCursor(0, 113);
  u8g2.print("Lo:");
  spr.drawFastHLine(0, 30, 239, TFT_CYAN);
  spr.drawFastVLine(95, 30, 105, TFT_CYAN);
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontSmall);
  spr.drawRightString(cHumidHi, 85, 55, GFXFF);
  spr.drawRightString(cHumidLo, 85, 95, GFXFF);
  spr.unloadFont();
  spr.loadFont(fontLarge);
  spr.drawRightString(cHumid, 239, 60, GFXFF);
  spr.unloadFont();
  spr.pushSprite(0, 0);
  spr.deleteSprite();
}

void writeBaro() {

  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_BLACK);
  u8g2.setForegroundColor(TFT_RED);
  u8g2.setCursor(0, 20);
  u8g2.print("Pressure (inHg)");
  u8g2.setFont(u8g2_font_helvR14_tr);
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Pressure")/2), 20, "Pressure");
  spr.drawFastHLine(0, 30, 240, TFT_CYAN);
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontLarge);
  spr.drawRightString(cBaro, 119, 60, GFXFF);
  spr.unloadFont();
  spr.pushSprite(0, 0, TFT_TRANSPARENT);
  spr.deleteSprite();
}

void transHumid() {

  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
  spr.createSprite(SPWIDTH, SPHEIGHT);
  u8g2.setForegroundColor(TFT_RED);
  u8g2.setFont(u8g2_font_helvR14_tf);
  u8g2.setFontMode(1);
  u8g2.setCursor(0, 20);
  u8g2.print("Temperature (°F)");
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Temperature")/2), 20, "Temperature");
  u8g2.setCursor(0, 73);
  u8g2.print("Hi:");
  u8g2.setCursor(0, 113);
  u8g2.print("Lo:");
  spr.drawFastHLine(0, 30, 239, TFT_CYAN);
  spr.drawFastVLine(95, 30, 105, TFT_CYAN);
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontSmall);
  spr.drawRightString(cTempHi, 85, 55, GFXFF);
  spr.drawRightString(cTempLo, 85, 95, GFXFF);
  spr.unloadFont();
  spr.loadFont(fontLarge);
  spr.drawRightString(cTempF, 239, 60, GFXFF);
  spr.unloadFont();
  spr1.createSprite(SPWIDTH, SPHEIGHT);
  u8g2x.setForegroundColor(TFT_RED);
  u8g2x.setFont(u8g2_font_helvR14_tr);
  u8g2x.setFontMode(1);
  u8g2x.setCursor(0, 20);
  u8g2x.print("Humidity (%)");
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Humidity")/2), 20, "Humidity");
  u8g2x.setCursor(0, 73);
  u8g2x.print("Hi:");
  u8g2x.setCursor(0, 113);
  u8g2x.print("Lo:");
  spr1.drawFastHLine(0, 30, 239, TFT_CYAN);
  spr1.drawFastVLine(95, 30, 105, TFT_CYAN);
  spr1.setTextColor(TFT_CYAN);
  spr1.loadFont(fontSmall);
  spr1.drawRightString(cHumidHi, 85, 55, GFXFF);
  spr1.drawRightString(cHumidLo, 85, 95, GFXFF);
  spr1.unloadFont();
  spr1.loadFont(fontLarge);
  spr1.drawRightString(cHumid, 239, 60, GFXFF);
  spr1.unloadFont();
  for (int ypos = SPHEIGHT; ypos > 0; ypos -= 2) {
    spr.pushSprite(0, (ypos - 2) - SPHEIGHT);
    spr1.pushSprite(0, ypos - 2);
  }
  spr.deleteSprite();
  spr1.deleteSprite();
}

void transBaro() {

  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_BLACK);
  u8g2.setForegroundColor(TFT_RED);
  u8g2.setFont(u8g2_font_helvR14_tr);
  u8g2.setFontMode(1);
  u8g2.setCursor(0, 20);
  u8g2.print("Humidity (%)");
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Humidity")/2), 20, "Humidity");
  u8g2.setCursor(0, 73);
  u8g2.print("Hi:");
  u8g2.setCursor(0, 113);
  u8g2.print("Lo:");
  spr.drawFastHLine(0, 30, 239, TFT_CYAN);
  spr.drawFastVLine(95, 30, 105, TFT_CYAN);
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontSmall);
  spr.drawRightString(cHumidHi, 85, 55, GFXFF);
  spr.drawRightString(cHumidLo, 85, 95, GFXFF);
  spr.unloadFont();
  spr.loadFont(fontLarge);
  spr.drawRightString(cHumid, 239, 60, GFXFF);
  spr.unloadFont();
  spr1.createSprite(SPWIDTH, SPHEIGHT);
  spr1.fillSprite(TFT_BLACK);
  u8g2x.setForegroundColor(TFT_RED);
  u8g2x.setCursor(0, 20);
  u8g2x.print("Pressure (inHg)");
  u8g2x.setFont(u8g2_font_helvR14_tr);
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Pressure")/2), 20, "Pressure");
  spr1.drawFastHLine(0, 30, 240, TFT_CYAN);
  spr1.setTextColor(TFT_CYAN);
  spr1.loadFont(fontLarge);
  spr1.drawRightString(cBaro, 119, 60, GFXFF);
  spr1.unloadFont();
  for (int ypos = SPHEIGHT; ypos > 0; ypos -= 2) {
    spr.pushSprite(0, (ypos - 2) - SPHEIGHT);
    spr1.pushSprite(0, ypos - 2);
  }
  spr.deleteSprite();
  spr1.deleteSprite();
}

void transTemp() {
  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_BLACK);
  u8g2.setForegroundColor(TFT_RED);
  u8g2.setCursor(0, 20);
  u8g2.print("Pressure (inHg)");
  u8g2.setFont(u8g2_font_helvR14_tr);
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Pressure")/2), 20, "Pressure");
  spr.drawFastHLine(0, 30, 240, TFT_CYAN);
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontLarge);
  spr.drawRightString(cBaro, 119, 60, GFXFF);
  spr.unloadFont();
  spr1.createSprite(SPWIDTH, SPHEIGHT);
  u8g2x.setForegroundColor(TFT_RED);
  u8g2x.setFont(u8g2_font_helvR14_tf);
  u8g2x.setFontMode(1);
  u8g2x.setCursor(0, 20);
  u8g2x.print("Temperature (°F)");
  //  u8g2.drawStr (119 - (u8g2.getUTF8Width("Temperature")/2), 20, "Temperature");
  u8g2x.setCursor(0, 73);
  u8g2x.print("Hi:");
  u8g2x.setCursor(0, 113);
  u8g2x.print("Lo:");
  spr1.drawFastHLine(0, 30, 239, TFT_CYAN);
  spr1.drawFastVLine(95, 30, 105, TFT_CYAN);
  spr1.setTextColor(TFT_CYAN);
  spr1.loadFont(fontSmall);
  spr1.drawRightString(cTempHi, 85, 55, GFXFF);
  spr1.drawRightString(cTempLo, 85, 95, GFXFF);
  spr1.unloadFont();
  spr1.loadFont(fontLarge);
  spr1.drawRightString(cTempF, 239, 60, GFXFF);
  spr1.unloadFont();
  for (int ypos = SPHEIGHT; ypos > 0; ypos -= 2) {
    spr.pushSprite(0, (ypos - 2) - SPHEIGHT);
    spr1.pushSprite(0, ypos - 2);
  }
  spr.deleteSprite();
  spr1.deleteSprite();
}

void drawScreen() {

   readSensors();                          // read the sensors and calculate values

   if (displayMode > 6) displayMode = 1;   // roll over the display mode back to 1

  switch (displayMode) {                   // select display mode
    case 1: writeTemp(); break;
    case 2: transHumid(); displayMode = 3; break;
    case 3: writeHumid(); break;
    case 4: transBaro(); displayMode = 5; break;
    case 5: writeBaro(); break;
    case 6: transTemp(); displayMode = 1; break;
  }
}

void loop() {                              // main program loop

button1.read();                            // read the button
long now = millis();
if (now - lastUpdate > 60000) {            // update the current display at this interval in ms
    lastUpdate = now;
    displayMode++;
    drawScreen();
  }
}

//end loop
