#include <LiquidCrystal_I2C.h> //https://codeload.github.com/johnrickman/LiquidCrystal_I2C/zip/master
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "WAC.h"
const char *ssid = SSID;
const char *password = PW;
//Create WAC.h to store creds
//#define SSID "Wireless Access Point"
//#define PW "Password"
const int analogInPin = A0;  // ESP8266 Analog Pin ADC0 = A0

int sensorValue = 0;  // value read from the pot
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 28800, 60000);

LiquidCrystal_I2C lcd(0x27, 16, 2);  

void setup(){
  Serial.begin(115200);
  lcd.init();                   
  lcd.backlight();
  lcd.print("System Armed");
  lcd.clear();     
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    lcd.setCursor(0, 0);
    lcd.print("Connecting to");
    lcd.setCursor(0, 1); 
    lcd.print(ssid); 
    Serial.print(".");
  }
  timeClient.begin();
  lcd.clear();
}

void loop(){
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(WiFi.localIP());
  sensorValue = analogRead(analogInPin);
  Serial.println(sensorValue);
  lcd.setCursor(0, 0);
  lcd.print(sensorValue);
  lcd.setCursor(8, 0);
  lcd.print(timeClient.getFormattedTime());
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(500);
}
