

#include <SPI.h>
#include "Adafruit_MAX31855.h"
#include "ThingSpeak.h"
#include "secrets.h"
#include <ESP8266WiFi.h>


char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

// initialize the Thermocouple
Adafruit_MAX31855 thermocouple(D6, D7, D1);

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

int field = 1;
int minutes =1;
int sensorNumber=1;

void setup() {
  Serial.begin(115200);

  while (!Serial) delay(1);
  delay(500);
  if (!thermocouple.begin()) {
    Serial.println("ERROR.");
    while (1) delay(10);
  }
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  Serial.println("");


 delay(sensorNumber*60000);
}

void loop() {
   Serial.println();
   Serial.print("Sensor:");
   Serial.println(sensorNumber);
   Serial.print("MAC:");
   Serial.println(WiFi.macAddress());

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){

    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass); 
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
  
   float c = thermocouple.readCelsius();
   if (isnan(c)) {
     Serial.println("Something wrong with thermocouple!");
   } else {
    Serial.println(c);
    int response =ThingSpeak.writeField(myChannelNumber, 1, c, myWriteAPIKey);
    if(response ==200){
     Serial.println("Channel updated succesfully");
    }else {
      Serial.println("Problem updating channel, error:"+String(response));
    }
     
   }

   delay(minutes*60000);
}
