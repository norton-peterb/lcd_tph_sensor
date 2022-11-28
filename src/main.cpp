#include <Arduino.h>
#include "TM1637Display.h"
#include "Adafruit_Sensor.h"
#include "Adafruit_BME280.h"

#define TM_CLK 3
#define TM_DIO 4
#define PR_CLK 5
#define PR_DIO 6
#define HU_CLK 7
#define HU_DIO 8

float temperature = 0.0f;
float pressure = 0.0f;
float humidity = 0.0f;

TM1637Display lcd = TM1637Display(TM_CLK,TM_DIO);
TM1637Display lcd_press = TM1637Display(PR_CLK,PR_DIO);
TM1637Display lcd_hum = TM1637Display(HU_CLK,HU_DIO);
Adafruit_BME280 bme;
bool status;

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
  status = bme.begin();  
  if (!status) {
    lcd.setSegments(error,3,0);
    lcd_press.setSegments(error,3,0);
    lcd_hum.setSegments(error,3,0);
  }
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
  }
  delay(30000);
}