#include <LiquidCrystal_I2C.h> //https://codeload.github.com/johnrickman/LiquidCrystal_I2C/zip/master
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>
#include "WAC.h"
const char *ssid = SSID;
const char *password = PW;
#define CLK D4 
#define DIO D3

TM1637Display display = TM1637Display(CLK, DIO); // Create display object of type TM1637Display:
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
  display.clear();
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
int A, B;

  timeClient.update();
  display.setBrightness(7); // Set the brightness:

  A = timeClient.getHours() * 100 + timeClient.getMinutes();
  B = timeClient.getSeconds();

  if ((B % 2) == 0)
  {
    display.showNumberDecEx(A, 0b01000000, false, 4, 0);
  }
  else
  {
    display.showNumberDecEx(A, 0b00000000, false, 4, 0);
  }
}
