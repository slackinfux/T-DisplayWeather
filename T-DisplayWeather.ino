// Weather sensor program for TTGO T-Display w/onboard TFT + BME280 sensor
#define FS_NO_GLOBALS
#define SPWIDTH  240
#define SPHEIGHT 135

#include <Adafruit_BME280.h>         // Include the BME280 library (works with 2.0.2)
#include <TFT_eSPI.h>
#include <U8g2_for_TFT_eSPI.h>
#include <EasyButton.h>
#include "Free_Fonts.h"

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
int Elevation = 549;                  //Elevation of current location in Meters
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
Adafruit_BME280 bme;
TFT_eSPI tft = TFT_eSPI();            // tft instance
U8g2_for_TFT_eSPI u8g2;               // U8g2 font instance
TFT_eSprite spr = TFT_eSprite(&tft);
EasyButton button1(0);

void setup() {

  tft.begin();                        // Enable the display
  tft.setRotation(3);
  u8g2.begin(tft);                    // Enable the Font renderer
  bme.begin(0x76, &Wire);             // Enable the bme280
  if (!SPIFFS.begin()) {
    while (1) yield();                          // Stay here twiddling thumbs waiting for SPIFFS
  }
  button1.begin();
  button1.onPressed(onPress1);
  TempHi = 0;
  TempLo = 150;
  HumidHi = 0;
  HumidLo = 100;
  tft.fillScreen(TFT_BLACK);
  drawScreen();

}
//end setup



void readSensors() {

  TempC = bme.readTemperature();
  Pa = bme.readPressure();
  Humid = bme.readHumidity();
  TempF = ((TempC * 1.8) + 32.0);
  SLPressure = (Pa) / pow((1 - ((float)(Elevation)) / 44330), 5.255); // Calculates sea level reference pressure from measured value and Elevation
  Baro = SLPressure / 3386.39;
//  TempF = 100;
//  Humid = 100;
  if (Humid > HumidHi) HumidHi = Humid;
  if (Humid < HumidLo) HumidLo = Humid; 
  if (TempF > TempHi) TempHi = TempF;
  if (TempF < TempLo) TempLo = TempF; 
  dtostrf(TempF, 2, 0, cTempF);
  dtostrf(TempHi, 2, 0, cTempHi);
  dtostrf(TempLo, 2, 0, cTempLo);
  dtostrf(Baro, 4, 2, cBaro);
  dtostrf(Humid, 2, 0, cHumid);
  dtostrf(HumidHi, 2, 0, cHumidHi);
  dtostrf(HumidLo, 2, 0, cHumidLo);    
}

// end readSensors

void onPress1() {
  // Do something when the button is pressed
  displayMode ++;
  tft.fillScreen(TFT_BLACK);
  drawScreen();
}
//end onPress1

void writeTemp() {
  
  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
//  tft.fillScreen(TFT_BLACK);
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
  tft.drawFastHLine(0, 30, 239, TFT_CYAN);
  tft.drawFastVLine(95, 30, 105, TFT_CYAN); 
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_TRANSPARENT);
  spr.setTextColor(TFT_CYAN);
  spr.fillRect(25, 31, 65, 100, TFT_BLACK);
  spr.loadFont(fontSmall);
  spr.drawRightString(cTempHi, 85, 55, GFXFF);
  spr.drawRightString(cTempLo, 85, 95, GFXFF);
  spr.unloadFont();
  spr.fillRect(96, 31, 239, 135, TFT_BLACK);
  spr.loadFont(fontLarge);
  spr.drawRightString(cTempF, 239, 60, GFXFF);
  spr.unloadFont();
  spr.pushSprite(0, 0, TFT_TRANSPARENT);
  spr.deleteSprite(); 

}

void writeHumid() {
  
  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
//  tft.fillScreen(TFT_BLACK);
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
  tft.drawFastHLine(0, 30, 239, TFT_CYAN);
  tft.drawFastVLine(95, 30, 105, TFT_CYAN); 
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_TRANSPARENT);
  spr.setTextColor(TFT_CYAN);
  spr.fillRect(25, 31, 65, 100, TFT_BLACK);
  spr.loadFont(fontSmall);
  spr.drawRightString(cHumidHi, 85, 55, GFXFF);
  spr.drawRightString(cHumidLo, 85, 95, GFXFF);
  spr.unloadFont();
  spr.fillRect(96, 31, 239, 135, TFT_BLACK);
  spr.loadFont(fontLarge);
  spr.drawRightString(cHumid, 239, 60, GFXFF);
  spr.unloadFont();
  spr.pushSprite(0, 0, TFT_TRANSPARENT);
  spr.deleteSprite(); 
}

void writeBaro() {
  
  // Draw Sensor Output

  String fontSmall = "Interceptor28";
  String fontLarge  = "Interceptor72";
//  tft.fillScreen(TFT_BLACK);
  u8g2.setForegroundColor(TFT_RED); 
  u8g2.setCursor(0, 20);
  u8g2.print("Pressure (inHg)");
  u8g2.setFont(u8g2_font_helvR14_tr);
//  u8g2.drawStr (119 - (u8g2.getUTF8Width("Pressure")/2), 20, "Pressure");
  tft.drawFastHLine(0, 30, 240, TFT_CYAN);
  spr.createSprite(SPWIDTH, SPHEIGHT);
  spr.fillSprite(TFT_TRANSPARENT);
  spr.fillRect(0, 31, 239, 134, TFT_BLACK);   
  spr.setTextColor(TFT_CYAN);
  spr.loadFont(fontLarge);
  spr.drawRightString(cBaro, 119, 60, GFXFF);
  spr.unloadFont();
  spr.pushSprite(0, 0, TFT_TRANSPARENT);
  spr.deleteSprite();
}

void drawScreen() {

   readSensors();  

   if (displayMode > 3) displayMode = 1;
   if (displayMode < 1) displayMode = 3;

  switch (displayMode) {
    case 1: writeTemp(); break;
    case 2: writeHumid(); break;
    case 3: writeBaro(); break;
  }
}

void loop() {                              // main program loop

button1.read();
long now = millis();
if (now - lastUpdate > 60000) {
    lastUpdate = now;
    drawScreen();
  }
}

//end loop
