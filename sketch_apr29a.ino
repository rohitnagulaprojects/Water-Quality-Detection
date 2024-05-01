#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <OneWire.h>
#include <DallasTemperature.h>

LiquidCrystal_I2C lcd(0x27, 2, 16);
#define ONE_WIRE_BUS 12
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int sensorPin = A0;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  sensors.begin();
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Welcome to Water");
  lcd.setCursor(0, 1);
  lcd.print("Quality System");
  delay(3000);

  int sensorValue = analogRead(sensorPin);
  int turbidity = map(sensorValue, 0, 750, 100, 0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Turbidity:");
  lcd.setCursor(11, 0);
  lcd.print(turbidity);
  lcd.print("%");
  lcd.setCursor(0, 1);
  if (turbidity < 20) {
    lcd.print("Water is Clean:)");
  } else if (turbidity < 50) {
    lcd.print("Water is Cloudy:|");
  } else {
    lcd.print("Water is Dirty:(");
  }

  delay(3000);

  sensors.requestTemperatures();
  float waterTempC = sensors.getTempCByIndex(0);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp:");
  lcd.setCursor(6, 0);
  lcd.print(waterTempC);
  lcd.print("`C");

  lcd.setCursor(0, 1);
  if (waterTempC < 25) {
    lcd.print("Water is Cool");
  } else if (waterTempC < 30) {
    lcd.print("Water is Normal");
  } else if (waterTempC < 40) {
    lcd.print("Water is Hot");
  } else {
    lcd.print("Water is Very Hot");
  }

  // Send values to ESP32 over Serial
  Serial.print("Turbidity:");
  Serial.print(turbidity);
  Serial.print(":Temperature:");
  Serial.println(waterTempC);

  delay(3000);
}
