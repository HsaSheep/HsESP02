/* /////////////////////////// */
/*  BeepTest And MacAddr_Send  */
/* 					            		   */
/* SSIDandPW Write and Upload  */
/* /////////////////////////// */

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>     //OTA
#include <ArduinoOTA.h>  //OTA

// in ~/Arduino/librarys/***/WIFI_SETTING.h
#include <WIFI_SETTING.h>
//#define SSID_DEFINE ()
//#define WIFI_PASSWD_DEFINE ()

const char* ssid = SSID_DEFINE;
const char* password = WIFI_PASSWD_DEFINE;

ESP8266WebServer server(80);

#define INCOMING_SIZE (5000)

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

char incomingChar[INCOMING_SIZE]={};
int incomingCount = 0;

void incoming_Clear(void)
{
  for(incomingCount=0; incomingCount<INCOMING_SIZE; incomingCount++)
  {
    incomingChar[incomingCount] = 0;
  }
  incomingCount = 0;
}

void handleRoot() {
  digitalWrite(led, 1);
  if(incomingCount == 0) server.send(200, "text/html", "hello from esp8266!</br></br>No Serial Recived(Cleared)...</br>");
  else server.send(200, "text/plain", incomingChar);
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
  //最低限のピン設定(LED,Beep等)OTA設定後にもう一回ピン設定
  pinMode(led, OUTPUT);
  pinMode(beep_pin,OUTPUT);
  digitalWrite(led, 1);
  beep(50);
  beep(50);
  beep(50);
  Serial.begin(115200);
  Serial.println("");
  Serial.println("----- Booting... -----");
  Serial.println("----- Serial Begin -----");
  Serial.print("  Mac Addr: ");
  Serial.println(mac_return());
  Serial.println(" !Enable OTA!");
  Serial.print("Connect to ");
  Serial.println(ssid);
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
      digitalWrite(led,1);
      delay(500);
      digitalWrite(led,0);
      delay(500);
      digitalWrite(led,1);
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
  //i2c  SDA:IO4 SCL:IO14
  Wire.begin(4, 14);
  delay(40);

  // MPU6050 SETTING
  uint8_t cmd_init[] = {0x38, 0x39, 0x14, 0x70, 0x56, 0x6c, 0x38, 0x0d, 0x01};
  command(cmd_init, sizeof(cmd_init));
  delayMicroseconds(1080);  // 1.08ms

  uint8_t cmd_str1[] = {0x48, 0x65, 0x6c, 0x6c, 0x6f};
  uint8_t cmd_cr[] = {0xc0};
  uint8_t cmd_str2[] = {0x57, 0x6f, 0x72, 0x6c, 0x64, 0x21};
  write(cmd_str1, sizeof(cmd_str1));
  command(cmd_cr, sizeof(cmd_cr));
  write(cmd_str2, sizeof(cmd_str2));
  // MPU6050 SETTING END
  
  digitalWrite(led,1);
  Serial.println("----- WiFi Connected! -----");
  Serial.print(" Connected: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("---------------------------");
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

  server.on("/serial", [](){

    server.send(200, "text/plain", incomingChar);
    // 「/」に転送
    //server.sendHeader("Location", String("/"), true);
    //server.send(302, "text/plain", "");

    Serial.println("----- incomingChar -----");
    for(int i=0;i<incomingCount;i++)
    {
      if((incomingChar[i] == 0x0A) || (incomingChar[i]) == 0x0D)
       Serial.println("");
      else
        Serial.print(incomingChar[i]);
    }
    Serial.println("");
    Serial.println("----- incomingByte -----");
    for(int i=0;i<incomingCount;i++)
    {
        Serial.print("[0x");
        Serial.print((byte)(incomingChar[i]));
        Serial.print("]");
    }
    Serial.println("");
    Serial.println("----- incoming Print END -----");
  });

  server.begin();
  Serial.println("HTTP server started");
  tone_(2093,500);
  tone_(4186,500);
  digitalWrite(led, 0);
  Serial.println("----- !!! StartUp Done !!! -----");
}

void loop(void){
  ArduinoOTA.handle(); // OTA
  server.handleClient();

  if(Serial.available() > 0)
  {
    if((incomingCount+Serial.available()) > INCOMING_SIZE)
    {
      Serial.println("Incoming Data Buffar Overed. Clear...");
      incoming_Clear();
     }
    else
    {
      while(Serial.available() > 0)
      {
        byte buff = Serial.read();
        if(0)
        {
          Serial.print("Char:");
          Serial.print((char)buff);
          Serial.print("| Byte:");
          Serial.println(buff);
        }
        //if(buff == '\c') incoming_Clear();
        if(buff == '\c') buff = ' ';
        else if(buff == '\0') buff = ' ';
        //else if(buff == 0x0D) buff = ' ';
        //else if(buff == 0x0A) buff = ' ';
        else incomingChar[incomingCount] = (char)(buff);
        
        incomingCount++;
      }
    }
  }
  
  if(digitalRead(12)  == LOW)
  {
    Serial.println("");
    Serial.println("---------- ReSendStatus ----------");
    Serial.print("  Mac Addr: ");
    Serial.println(mac_return());
    Serial.print(" Connected: ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    tone_(2093,150);
    tone_(4186,300);
  }
  else if(digitalRead(0) == LOW)
  {
    incoming_Clear();
    tone_(4000,50);
    tone_(8000,50);
    tone_(4000,50);
  }
}
