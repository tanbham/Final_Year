#include <ESP8266WiFi.h>
String msg = "";
const char* ssid = "vivo 1917";
const char* password = "chinmay123";
WiFiServer wifiServer(5555);

//ultrasonic
//const int trigp1 = 5; //d1
const int trigp2 = 15; //d8
//const int echop1 = 4; //d2
const int echop2 = 13; //d7
int duration = 0;
int distanceL;
int distanceR;
int distance;

//motor
int motor1pin1 = 5; //d1
int motor1pin2 = 4; //d2
int motor2pin1 = 0; //d3
int motor2pin2 = 2; //d4
int enable1 = 9;  //sd2
int enable2 = 10; //sd3
int duty = 255;
int zero = 0;

int calculate_pwm(){
  while(duty > 200){
    Serial.println(duty);
    analogWrite(motor1pin1, duty);
    analogWrite(motor1pin2, zero);
    analogWrite(motor2pin1, duty);
    analogWrite(motor2pin2, zero);
    duty = duty - 20;
  }
  return duty;
}

  
int ultrasonic(int trig , int echo){
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  Serial.println(duration);
  Serial.println(distance);
  distance = duration*0.034/2;
  return distance;
}

void left(int duty){
   analogWrite(motor1pin1, duty);
   analogWrite(motor1pin2, zero);
   analogWrite(motor2pin1, zero);
   analogWrite(motor2pin2, zero);  
}

void forward(int duty){
   analogWrite(motor1pin1, duty);
   analogWrite(motor1pin2, zero);
   analogWrite(motor2pin1, duty);
   analogWrite(motor2pin2, zero);  
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
        //duty = calculate_pwm();         //speed reduced to 150
        //forward(duty);                  //drive with new slow speed
        //delay(1000);

        while(1){                       //continuous sensing of left lane cars
          //distanceR = ultrasonic(trigp1 , echop1);
          // delay(60);
          distanceL = ultrasonic(trigp2 , echop2);

          //if(distanceL < 20 || distanceR < 20){
          //  forward(duty);
           // left(0);
            //Serial.println("forward...");
          //}
        
          //else{
           // forward(0);
            //left(duty); 
            //Serial.println("left...");
         // }
        
          //Serial.println("Distance Right = ");
          //Serial.println(distanceR);
          Serial.println("Distance left = ");
          Serial.println(distanceL);     
        }
      }
    }
  }
 Serial.println("System stop"); 
}
