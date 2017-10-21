/* //////////////////////////////
/*  BeepTest And MacAddr_Send  //
/* 					            		   //
/* SSIDandPW Write and Upload  //
/* ////////////////////////////*/

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

const char* ssid = "WiFi-SSID";
const char* password = "WiFi-Password";

ESP8266WebServer server(80);

const int led = 13;
const int beep_pin =  5;

void beep(unsigned char delayms,unsigned char val = 20){
  analogWrite(beep_pin, val);      // Almost any value can be used except 0 and 255
                                  // experiment to get the best tone
  delay(delayms);                 // wait for a delayms ms
  analogWrite(beep_pin, 0);       // 0 turns it off
  delay(delayms);                 // wait for a delayms ms
  digitalWrite(beep_pin, 0);
}

void tone_(unsigned int hz,unsigned int delayms = 100)
{
  tone(beep_pin, hz, delayms);
  delay(delayms);
  digitalWrite(beep_pin, 0);
}

char* mac_return(void)
{
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
  return buff;
}

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  digitalWrite(led, 0);
}

void handleNotFound(){
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

void setup(void){
  pinMode(led, OUTPUT);
  pinMode(beep_pin,OUTPUT);
  digitalWrite(led, 1);
  beep(50);
  beep(50);
  beep(50);
  digitalWrite(led, 0);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("----- Serial Begin -----");
  Serial.print("  Mac Addr: ");
  Serial.println(mac_return());
  WiFi.begin(ssid, password);
  Serial.println("----- WiFi Connect wait... -----");

  // Wait for connection
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
  }
  digitalWrite(led,1);
  Serial.println("");
  Serial.println("----- WiFi Connected! -----");
  Serial.print(" Connected: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  tone_(262,500);
  tone_(523,500);

  // Begin mDNS
  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  // Server On
  server.on("/", handleRoot);

  server.on("/inline", [](){
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
  tone_(2093,500);
  tone_(4186,500);
  digitalWrite(led, 0);
  Serial.println("----- !!! StartUp Done !!! -----");
}


void loop(void){
  server.handleClient();
  if(digitalRead(12) == LOW)
  {
    tone_(2093,150);
    tone_(4186,300);

    Serial.println("");
    Serial.println("---------- ReSendStatus ----------");
    Serial.print("  Mac Addr: ");
    Serial.println(mac_return());
    Serial.print(" Connected: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}
