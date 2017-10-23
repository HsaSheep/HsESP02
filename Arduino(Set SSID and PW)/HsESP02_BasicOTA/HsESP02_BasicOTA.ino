#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "WIFI_SETTING.h"
//#define SSID_DEFINE ()
//#define WIFI_PASSWD_DEFINE ()

const char* ssid = "SSID_DEFINE";
const char* password = "WIFI_PASSWD_DEFINE";

const int led = 13;
const int beep_pin =  16;

void beep(unsigned char delayms,unsigned char val = 20){
  analogWrite(beep_pin, val);      // Almost any value can be used except 0 and 255
                                  // experiment to get the best tone
  delay(delayms);                 // wait for a delayms ms
  analogWrite(beep_pin, 0);       // 0 turns it off
  delay(delayms);                 // wait for a delayms ms
  digitalWrite(beep_pin, 0);
}

void setup() {
  byte mac_addr[6];
  static char buff[20];
  WiFi.macAddress(mac_addr);
  sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X",
  mac_addr[0],
  mac_addr[1],
  mac_addr[2],
  mac_addr[3],
  mac_addr[4],
  mac_addr[5]);

  pinMode(led, OUTPUT);
  pinMode(beep_pin,OUTPUT);
  digitalWrite(led, 1);
  beep(50);
  beep(50);
  beep(50);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("---- Booting... ---");
  Serial.println("----- Serial Begin -----");
  Serial.print("  Mac Addr: ");
  Serial.println(buff);
  Serial.println(" !Enable OTA(BASIC_MODE)!");
  WiFi.mode(WIFI_STA); // OTA
  WiFi.begin(ssid, password);
  Serial.println("----- WiFi Connect wait... -----");

  // Wait for connection
  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(led, 1);
    delay(100);
    digitalWrite(led, 0);
    delay(100);
    digitalWrite(led, 1);
    delay(100);
    digitalWrite(led, 0);
    delay(700);
    Serial.print(".");
    counter++;
    if(counter > (60 * 5)) //5分以上経過
    {
      Serial.println("Connection Failed! Rebooting...");
      digitalWrite(led, 1);
      delay(500);
      digitalWrite(led, 0);
      delay(500);
      digitalWrite(led, 1);
      delay(500);
      ESP.restart();
    }
  }
// ---------- OTA setup's ---------- //
// Port defaults to 8266
  // ArduinoOTA.setPort(8266);

  // Hostname defaults to esp8266-[ChipID]
  ArduinoOTA.setHostname("HsESP02_v4");

  // No authentication by default
  // ArduinoOTA.setPassword((const char *)"123");

  ArduinoOTA.onStart([]() {
    Serial.println("Start");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("");
  Serial.println("OTA Ready.");
// ------------------------------------ //

// ピン設定
  
  digitalWrite(led,1);
  Serial.println("");
  Serial.println("----- WiFi Connected! -----");
  Serial.print(" Connected: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  beep(50);
  beep(50);
  digitalWrite(led, 0);
  Serial.println("----- !!! StartUp Done (OTA ONLY MODE) !!! -----");
}

void loop() {
  ArduinoOTA.handle(); // OTA
}
