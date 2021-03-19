#include <LiquidCrystal_I2C.h> //https://codeload.github.com/johnrickman/LiquidCrystal_I2C/zip/master
#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TM1637Display.h>
#include <ESPAsyncWebServer.h>
#include "WAC.h"
#include "GPIO.h"
#include "WP.h"
#include "relayState.h"
#include "processor.h"
const char *ssid = SSID;
const char *password = PW;
//Create WAC.h to store creds
//#define SSID "Wireless Access Point"
//#define PW "Password"
TM1637Display display = TM1637Display(CLK, DIO); // Create display object of type TM1637Display:
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 28800, 60000);

LiquidCrystal_I2C lcd(0x27, 16, 2);  
const char* PARAM_INPUT_1 = "relay";  
const char* PARAM_INPUT_2 = "state";
// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


void setup(){
  Serial.begin(115200);
  pinMode(D4, OUTPUT);
  for(int i=1; i<=NUM_RELAYS; i++){
    pinMode(relayGPIOs[i-1], OUTPUT);
    if(RELAY_NO){
      digitalWrite(relayGPIOs[i-1], HIGH);
    }
    else{
      digitalWrite(relayGPIOs[i-1], LOW);
    }
  }
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
  Serial.println(WiFi.localIP());
  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?relay=<inputMessage>&state=<inputMessage2>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    String inputMessage2;
    String inputParam2;
    // GET input1 value on <ESP_IP>/update?relay=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1) & request->hasParam(PARAM_INPUT_2)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      inputMessage2 = request->getParam(PARAM_INPUT_2)->value();
      inputParam2 = PARAM_INPUT_2;
      if(RELAY_NO){
        Serial.print("NO ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], !inputMessage2.toInt());
      }
      else{
        Serial.print("NC ");
        digitalWrite(relayGPIOs[inputMessage.toInt()-1], inputMessage2.toInt());
      }
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage + inputMessage2);
    request->send(200, "text/plain", "OK");
  });
  // Start server
  server.begin();
  timeClient.begin();
  lcd.clear();
}

void loop(){
  digitalWrite(D4, HIGH);
  timeClient.update();
  Serial.println(timeClient.getFormattedTime());
  Serial.println(WiFi.localIP());
  lcd.setCursor(0, 0);
  lcd.print("Local IP Address");
  lcd.setCursor(0,1);
  lcd.print(WiFi.localIP());
  delay(500);
  int A, B;
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
  digitalWrite(D4, LOW);
}
