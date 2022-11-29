#include <Arduino.h>
#include <Wire.h>
#include "TM1637Display.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

#define TM_CLK 3
#define TM_DIO 4
#define PR_CLK 5
#define PR_DIO 6
#define HU_CLK 7
#define HU_DIO 8
#define AG_CLK 9
#define AG_DIO 10

float temperature = 0.0f;
float pressure = 0.0f;
float humidity = 0.0f;
uint16_t gas = 0;

TM1637Display lcd = TM1637Display(TM_CLK,TM_DIO);
TM1637Display lcd_press = TM1637Display(PR_CLK,PR_DIO);
TM1637Display lcd_hum = TM1637Display(HU_CLK,HU_DIO);
TM1637Display lcd_gas = TM1637Display(AG_CLK,AG_DIO);
Adafruit_BME280 bme;
bool status;
char buffer[32];
char valBuffer[8];

const uint8_t celsius[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,  // Degree symbol
  SEG_A | SEG_D | SEG_E | SEG_F   // C
};
const uint8_t fahrenheit[] = {
	SEG_A | SEG_B | SEG_F | SEG_G,  // Circle
	SEG_A | SEG_E | SEG_F | SEG_G   // F
};
const uint8_t percent[] = {
  SEG_A | SEG_B | SEG_F | SEG_G,
  SEG_C | SEG_D | SEG_E | SEG_G
};

const uint8_t error[] = {
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,
  SEG_E | SEG_G,
  SEG_E | SEG_G
};

void setup() {
  lcd.setBrightness(3);
  lcd.clear();
  lcd_press.setBrightness(3);
  lcd_press.clear();
  lcd_hum.setBrightness(3);
  lcd_hum.clear();
  lcd_gas.setBrightness(3);
  lcd_gas.clear();
  status = bme.begin(0x76);  
  if (!status) {
    lcd.setSegments(error,3,0);
    lcd_press.setSegments(error,3,0);
    lcd_hum.setSegments(error,3,0);
    lcd_gas.setSegments(error,3,0);
  }
}

void sendData() {
  Wire.beginTransmission(0x04);
  dtostrf(temperature,-1,1,valBuffer);
  sprintf(buffer,"TMP|%s\n",valBuffer);
  Wire.write(buffer);
  dtostrf(pressure,-1,1,valBuffer);
  sprintf(buffer,"PRS|%s\n",valBuffer);
  Wire.write(buffer);
  dtostrf(humidity,-1,1,valBuffer);
  sprintf(buffer,"HUM|%s\n",valBuffer);
  Wire.write(buffer);
  Wire.endTransmission();
}

void loop() {
  if(status) {
    temperature = bme.readTemperature();
    pressure = bme.readPressure() / 100.0f;
    humidity = bme.readHumidity();
    lcd.clear();
    lcd.showNumberDec((int)temperature,false,2,0);
    lcd.setSegments(celsius, 2, 2);
    lcd_press.clear();
    lcd_press.showNumberDec((int)pressure,false,4,0);
    lcd_hum.clear();
    lcd_hum.showNumberDec((int)humidity,false,2,0);
    lcd_hum.setSegments(percent,2,2);
    gas = analogRead(A0);
    lcd_gas.clear();
    lcd_gas.showNumberDec(gas,true,4,0);
    sendData();
  }
  delay(30000);
}