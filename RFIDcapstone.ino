#include <SPI.h>
#include <MFRC522.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define SS_PIN D4
#define RST_PIN D2

#define ERROR_PIN D0
#define SUCCESS_PIN D1
#define CONN_PIN D8
#define BUZZER_PIN D3


const char *ssid = "TP-Link_3838";
const char *password = "nickaikian2143";

MFRC522 mfrc522(SS_PIN, RST_PIN);
void setup() {
   delay(1000);
   Serial.begin(9600);
   WiFi.mode(WIFI_OFF);    
   delay(1000);
   WiFi.mode(WIFI_STA);
   WiFi.begin(ssid, password);
   Serial.println("");

   pinMode(CONN_PIN, OUTPUT);
   pinMode(SUCCESS_PIN, OUTPUT);
   pinMode(ERROR_PIN, OUTPUT);
   pinMode(BUZZER_PIN, OUTPUT);
   
   Serial.print("Connecting to ");
   Serial.println(ssid);
   while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
   }

   Serial.println("");
   Serial.print("Connected to ");
   Serial.println(ssid);
   Serial.print("IP address: ");
   Serial.println(WiFi.localIP()); 
   
   SPI.begin();
   mfrc522.PCD_Init();
}

void sendRfidLog(long cardid) {
  
  if(WiFi.status() == WL_CONNECTED) {
    WiFiClient client; 
    HTTPClient http;
    String postData = "cardid=" + String(cardid);
    http.begin(client, "http://192.168.0.108:3000/database");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");  
    int httpCode = http.POST(postData); 
    String payload = http.getString();
    Serial.println(httpCode);
    Serial.println(payload);
    
    if(payload.equals("success")) {
      digitalWrite(SUCCESS_PIN, HIGH);
      digitalWrite(BUZZER_PIN, HIGH);  // Activate the buzzer
      analogWrite(BUZZER_PIN, 50);  // Adjust the value (0-255) for volume
      delay(500);  // Buzzer activation duration (adjust as needed)
      digitalWrite(BUZZER_PIN, LOW);   // Deactivate the buzzer
    } else {
      digitalWrite(ERROR_PIN, HIGH);
    }
    
    http.end();
  }
}



void toggleConnStat() {
  if(WiFi.status() == WL_CONNECTED) {
    digitalWrite(CONN_PIN, HIGH);
  } else {
    digitalWrite(CONN_PIN, LOW);
  }
}

void loop() {

  if ( mfrc522.PICC_IsNewCardPresent()){
    if ( mfrc522.PICC_ReadCardSerial()){
      long code=0;
      for (byte i = 0; i < mfrc522.uid.size; i++){
        code=((code+mfrc522.uid.uidByte[i])*10);
      }
      sendRfidLog(code);
    }
  }
  
  toggleConnStat();
  delay (500);
  
  digitalWrite(SUCCESS_PIN, LOW);
  digitalWrite(ERROR_PIN, LOW);
     
}