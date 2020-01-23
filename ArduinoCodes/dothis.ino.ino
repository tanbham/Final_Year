//include <WiFi.h>
#include <ESP8266WiFi.h>
#define LED D4
String msg = "";
const char* ssid = "vivo 1917";
const char* password = "chinmay123";
WiFiServer wifiServer(5555);

//motor
int motor1pin1 = 5;
int motor1pin2 = 4;
int motor2pin1 = 14;
int motor2pin2 = 12;
int enable1 = 0;
int enable2 = 2;
int duty = 255;
int zero = 0;

void stop(){
  digitalWrite(motor1pin1 ,LOW);
  digitalWrite(motor1pin2 ,LOW);
  digitalWrite(motor2pin1 ,LOW);
  digitalWrite(motor2pin2 ,LOW);
}

void setup() {
 pinMode(motor1pin1,OUTPUT);
 pinMode(motor1pin2,OUTPUT);
 pinMode(motor2pin1,OUTPUT);
 pinMode(motor2pin2,OUTPUT);
 pinMode(enable1,OUTPUT);
 pinMode(enable2,OUTPUT); 
 Serial.begin(115200);
 delay(1000);
  
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
 }
 Serial.println("Connected to the WiFi network");
 Serial.println(WiFi.localIP());
 wifiServer.begin();
}

void loop() {
  WiFiClient client = wifiServer.available();
  if (client){
    while (client.connected()){
      msg="";
      while (client.available()>0){
        char c = client.read();
        msg += c;
      }
      Serial.println(msg);
      
      //turn on the motors
      digitalWrite(enable1 , HIGH);
      digitalWrite(enable2 , HIGH);
      analogWrite(motor1pin1, duty);
      analogWrite(motor1pin2, zero);
      analogWrite(motor2pin1, duty);
      analogWrite(motor2pin2, zero);
      Serial.println("waiting for drowsiness");
      
      if(msg == "drowsy"){
        while(duty > 50){
          Serial.println(duty);
          analogWrite(motor1pin1, duty);
          analogWrite(motor1pin2, zero);
          analogWrite(motor2pin1, duty);
          analogWrite(motor2pin2, zero);
          duty = duty - 20;
          delay(1000);
        }
        break;      
      }
     
    }
  }
}
