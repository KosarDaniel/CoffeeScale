/*
   -------------------------------------------------------------------------------------
   HX711_ADC
   Arduino library for HX711 24-Bit Analog-to-Digital Converter for Weight Scales
   Olav Kallhovd sept2017
   -------------------------------------------------------------------------------------
*/

/*
   Settling time (number of samples) and data filtering can be adjusted in the config.h file
   For calibration and storing the calibration value in eeprom, see example file "Calibration.ino"

   The update() function checks for new data and starts the next conversion. In order to acheive maximum effective
   sample rate, update() should be called at least as often as the HX711 sample rate; >10Hz@10SPS, >80Hz@80SPS.
   If you have other time consuming code running (i.e. a graphical LCD), consider calling update() from an interrupt routine,
   see example file "Read_1x_load_cell_interrupt_driven.ino".

   This is an example sketch on how to use this library
*/
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

//pins:
const int HX711_dout = 8; //mcu > HX711 dout pin
const int HX711_sck = 9; //mcu > HX711 sck pin

//HX711 constructor:
HX711_ADC LoadCell(HX711_dout, HX711_sck);

const int calVal_eepromAdress = 0;
unsigned long t = 0;
float iii = 0;
float ii = 0;
float i = 0;

unsigned long ti = 0;

bool mode = 0; //Mode 0 is for fluid out weight, Mode 1 is for grind weight 

const float portafilter = 170; //Portafilter weight in grams

void setup() {
  Serial.begin(57600); delay(10);
  Serial.println();
  Serial.println("Starting...");

  LoadCell.begin();
  //LoadCell.setReverseOutput(); //uncomment to turn a negative output value to positive
  float calibrationValue; // calibration value (see example file "Calibration.ino")
  calibrationValue = 696.0; // uncomment this if you want to set the calibration value in the sketch
#if defined(ESP8266)|| defined(ESP32)
  //EEPROM.begin(512); // uncomment this if you use ESP8266/ESP32 and want to fetch the calibration value from eeprom
#endif
  //EEPROM.get(calVal_eepromAdress, calibrationValue); // uncomment this if you want to fetch the calibration value from eeprom

  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Timeout, check MCU>HX711 wiring and pin designations");
    while (1);
  }
  else {
    LoadCell.setCalFactor(calibrationValue); // set calibration value (float)
    Serial.println("Startup is complete");
  }
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();

  display.setTextSize(2);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
  display.setCursor(0, 0);     // Start at top-left corner
  display.cp437(true);         // Use full 256 char 'Code Page 437' font

  pinMode(7, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(7), reset, RISING);

  drawCoordinate();
  display.display;
}

void loop() {
  static boolean newDataReady = 0;
  const int serialPrintInterval = 100; //increase value to slow down serial print activity

  // check for new data/start next conversion:
  if (LoadCell.update()) newDataReady = true;

  // get smoothed value from the dataset:
  if (newDataReady) {
    if (millis() > t + serialPrintInterval) {
      iii = ii;
      ii = i;
      i = LoadCell.getData();
      //display.clearDisplay();
      display.fillRect(0, 0, 64, 64, SSD1306_BLACK);
      //i = i*0.6482;
      i = (i*0.5+ii+iii*1.5)/3;
      String str = String(i,1);
      if(i<10 && i>=0) str = "0"+str;

      if(mode == 0){
        display.setCursor(0, 0);
        display.print(str);
        display.setTextSize(1);
        display.print("g");
        display.setTextSize(2);
        display.setCursor(0, 16);
        display.print(String((millis()-ti)/1000));
        display.setTextSize(1);
        display.print("s");
        display.setTextSize(2);
        /*
        Serial.print(iii);
        Serial.print(" ");
        Serial.print(ii);
        Serial.print(" ");
        Serial.println(i);
        display.setCursor(64,0);
        str = String((i-iii)*5,1);
        display.print(str);
        display.setTextSize(1);
        display.print("g/s");
        display.setTextSize(2);
        */
        if(31-i*0.5-3 >= 0 && 31-i*0.5-3 <= 28){
          display.drawPixel(64+(millis()-ti)/500, int(31-i*0.5)-3, SSD1306_WHITE);
          }
      }else{
        display.clearDisplay;
        str = String(i-portafilter,1);
        if(i-portafilter<10 && i-portafilter>=0) str = "0"+str;
        display.setCursor(0, 0);
        display.setTextSize(2);
        display.print(str);
        display.setTextSize(1);
        display.print("g");
      }
      display.display();
      newDataReady = 0;
      t = millis();
    }
  }
}
void reset(){
  //LoadCell.tareNoDelay();
  ti = millis();
  display.clearDisplay();
  drawCoordinate();
  //display.display();
  attachInterrupt(digitalPinToInterrupt(7), 3sec, FALLING);
  }
 
void 3sec(){
  if(ti>3000) changeMode();
  attachInterrupt(digitalPinToInterrupt(7), reset, RISING);
  }

void changeMode(){
  LoadCell.tareNoDelay();
  mode = mode!;
  }

void drawCoordinate(){
  display.drawLine(64, 29, 127, 29, SSD1306_WHITE);
  for(int j = 0; j < 17; j++){
    display.drawPixel(64+j*4, 30, SSD1306_WHITE);
    display.drawPixel(64+j*4, 31, SSD1306_WHITE);
    }
  }
