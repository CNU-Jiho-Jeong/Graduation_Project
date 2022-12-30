#include <Servo.h> 
Servo servo1;
Servo servo2;


#define trig 7
#define echo 6
int angle = 0;
int angle_1 = 0;





void setup() 
{
  servo1.attach(2);
  servo2.attach(3);
  Serial.begin(9600);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  

  
}

void loop() 
{
 long duration, distance;
 
 
  digitalWrite(trig, LOW);
  digitalWrite(echo, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  

  duration = pulseIn(echo, HIGH);
  distance = ((float) (duration*340) / 10000) / 2;

  Serial.print("거리 : ");
  Serial.print(distance);
  Serial.println("cm");
  

  if (distance > 50) 
  {
    angle = 60;
    angle_1 = 120;
    delay(500);
    servo1.write(angle_1);
    servo2.write(angle);
  }
  else
{
    angle = 90;
    delay(500);
    servo1.write(angle);
    servo2.write(angle);
  }


}


  
