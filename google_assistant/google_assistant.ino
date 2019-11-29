/***************************************************
 Adafruit MQTT Library ESP8266 Example
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#define pin0 0 // GPIO 0
#define pin1 1 // TX Pin
#define pin2 2 // GPIO 2
/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "samer"
#define WLAN_PASS       "12345678"

/************************* Adafruit.io Setup *********************************/
//https://io.adafruit.com/*****************
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883    // use 8883 for SSL
#define AIO_USERNAME    "SamerAyash"   //Your Username
#define AIO_KEY         "*****************************" // Your API Key
/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

/****************************** Feeds ***************************************/
//////// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe onoff0 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/io0");
Adafruit_MQTT_Subscribe onoff2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/io2");
Adafruit_MQTT_Subscribe onofftx = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/rx");
Adafruit_MQTT_Subscribe securityMode = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/securityMode");

/*************************** Sketch Code ************************************/

void MQTT_connect();

void setup() {
  Serial.begin(115200);
  delay(10);
 pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
    
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(2, OUTPUT);

  digitalWrite(0, LOW);
  digitalWrite(1, LOW);
  digitalWrite(2, LOW);
  Serial.println(F("Adafruit MQTT demo"));
 Serial.print("Connecting to ");
 Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID,WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
   Serial.println(".");
   Serial.println("Wifi is not yet connected");
  }
 Serial.println();
  Serial.println("WiFi connected");
 Serial.println("IP address: "); Serial.println(WiFi.localIP());
   // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoff0);
  mqtt.subscribe(&onoff2);
  mqtt.subscribe(&onofftx);
  mqtt.subscribe(&securityMode);
   delay(1000);
  digitalWrite(3, HIGH);
 
}

uint32_t x=0;
 int s=0;
void loop() {
  MQTT_connect();

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &onoff0){
      uint16_t state0 = atoi((char *)onoff0.lastread);
      Serial.println(state0);
      delay(300);
      digitalWrite(pin0,state0);
    }
  
    if (subscription == &onoff2){
      uint16_t state2 = atoi((char *)onoff2.lastread);
      delay(300);
      digitalWrite(pin2,state2);
    }
    if (subscription == &onofftx){
      uint16_t staterx = atoi((char *)onofftx.lastread);
      delay(300);
      digitalWrite(pin1,staterx);
    }
    
  }
  /**************************Security Mode*********************/
 /* uint16_t modeonoff = atoi((char *)securityMode.lastread);
   if( modeonoff == 1 ){
    if(digitalRead(pin1)==HIGH){
    if(s == 0){
          Adafruit_MQTT_Publish temp = Adafruit_MQTT_Publish(&mqtt,AIO_USERNAME "/feeds/message");
         temp.publish(1);
         s =1;
        }
      }
  }  
 else{
          s= 0;
          }*/
 /**************************Security Mode*********************/

 }

 
// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
 Serial.print("Connecting to MQTT... ");
  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  {
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
         while (retries == 0){;
         Serial.println("MQTT is not Connected!");
          digitalWrite(pin0,HIGH);
          digitalWrite(pin1,HIGH);
          digitalWrite(pin2,HIGH);
       }
  }
 Serial.println("MQTT Connected!");
}
