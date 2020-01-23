const int trigP = 15;  //D1 Or GPIO-2 of nodemcu
const int echoP = 13;  //D2 Or GPIO-0 of nodemcu
const int led = 14;//D5
int duration = 0;
int distance;



void setup() {
  pinMode(led, OUTPUT);
  pinMode(trigP, OUTPUT);  // Sets the trigPin as an Output
  pinMode(echoP, INPUT); 
  Serial.begin(115200);// put your setup code here, to run once:

}

void loop() {
digitalWrite(trigP, LOW);   // Makes trigPin low
delayMicroseconds(2);       // 2 micro second delay 

digitalWrite(trigP, HIGH);  // tigPin high
delayMicroseconds(10);      // trigPin high for 10 micro seconds
digitalWrite(trigP, LOW);   // trigPin low

duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds

distance= duration*0.034/2;        //Calculating actual/real distance
if(distance<10){
  digitalWrite(led,HIGH);
  Serial.println("glow");
  Serial.print("Distance = ");        //Output distance on arduino serial monitor 
  Serial.println(distance);
}
else{
  digitalWrite(led,LOW);
  Serial.println("dont glow");
  Serial.print("Distance = ");        //Output distance on arduino serial monitor 
  Serial.println(distance);
}
delay(400);        
}
