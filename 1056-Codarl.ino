#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <NTPClient.h>


int HOURS_PINS[5] = {2, 0, 4, 5, 16};
//int HOURS_PINS[5] = {16, 5, 4, 0, 2};
int MINUTES_PINS[6] = {1, 3, 15, 13, 12, 14};
//int MINUTES_PINS[6] = {14, 12, 13, 15, 3, 1};

int HOURS = 0;
int MINUTES = 0;

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "asia.pool.ntp.org", 28800, 60000);



void connectToWiFi(){

char* ssid = "";  //wifi ssid
char* password = "";   //wifi password

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

}


void updateTime(int *hours, int *minutes){

    
    String formattedTime;
    String stringHours;
    String stringMinutes;

    //get timestamp
    while(!timeClient.update()) {
        timeClient.forceUpdate();
    }
    formattedTime = timeClient.getFormattedTime();

    //convert string timestamp to int
    stringHours = formattedTime.substring(0, 2);
    stringMinutes = formattedTime.substring(3, 5);

    *hours = atoi(stringHours.c_str());
    *minutes = atoi(stringMinutes.c_str());
}

void setup() {

  //start serial communication
  Serial.begin(9600);

    //connect to wifi 
    connectToWiFi();

    timeClient.begin();
  
    
  //set led pin's as output
  for(int i = 0; i < 5; i++)
      pinMode(HOURS_PINS[i], OUTPUT);
  for(int i = 0; i < 6; i++)
      pinMode(MINUTES_PINS[i], OUTPUT); 
  
    

}

void loop() {

    Serial.println("\n------------------------------------------\n");

    //update hours & seconds    
    updateTime(&HOURS, &MINUTES);
    
    Serial.print("Hours : ");
    Serial.println(HOURS);

    Serial.print("Binary : ");
    //convert hours to binary
    for(int i = 0; i < 5; i++){
        digitalWrite(HOURS_PINS[i], HOURS % 2);
        Serial.print(HOURS % 2);
        HOURS /= 2;
    }
    Serial.println();
    Serial.println();

    Serial.print("Minutes : ");
    Serial.println(MINUTES);
    Serial.print("Binary : ");
    //convert minutes to binary
    for(int i = 0; i < 6; i++){
        digitalWrite(MINUTES_PINS[i], MINUTES % 2);
        Serial.print(MINUTES % 2);
        MINUTES /= 2;
    }
    Serial.println();
    Serial.println();
  
  delay(1000);
}
