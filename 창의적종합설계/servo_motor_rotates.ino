#include <Servo.h> // 서보모터 라이브러리 불러오기 

// Servo 객체명; -> 여기서 객체명은 Servo 명령어를 따르는 서보모터 이름
Servo servo1; // servo1 서보모터
Servo servo2; // servo2 서보모터


#define trig 7 // trig는 초음파 발사부분 / trig를 7번 핀으로 정의하여 사용
#define echo 6 // echo는 초음파 받는 부분 / echo를 6번 핀으로 정의하여 사용
int angle = 0; // angle 변수 정의 (정수, 초기값 0) -> servo2 모터 회전 각
int angle_1 = 0; // angle_1 변수 정의 (정수, 초기값 0) -> servo1 모터 회전 각





void setup() 
{
  servo1.attach(2); // servo1은 아두이노 2번 핀에 연결
  servo2.attach(3); // servo2는 아두이노 3번 핀에 연결
  Serial.begin(9600); // setup()함수 안에 작성 / Serial 통신 사용을 선언 / 9600은 통신속도를 의미
  pinMode(trig, OUTPUT); // trig를 출력으로 설정
  pinMode(echo, INPUT); // echo를 입력으로 설정

  

  
}

void loop() // 반복 
{
 long duration, distance; // long: 정수를 저장하는 변수 / 2^32 가지 정수값 표현 가능
 
 
  digitalWrite(trig, LOW); // digitalWrite(핀 번호, HIGH 아니면 LOW) -> 핀 번호에 해당하는 핀에 전류를 on 아니면 off 시켜준다.
  digitalWrite(echo, LOW); // 여기서 trig, echo 둘 다 전류 off
  delayMicroseconds(2); // 마이크로 초단위 만큼 대기. trig에 전류를 on 해주기 전에 잠깐 대기해주는 것이다.  
  digitalWrite(trig, HIGH); // trig 전류 on
  delayMicroseconds(10); // 전류 off 해주기 전에 잠깐 대기해준다. 바로 전류를 off하면 초음파를 발사하지 못할 수 있기 때문이다. 
  digitalWrite(trig, LOW);
  

  duration = pulseIn(echo, HIGH); 
  // pulseIn: 아두이노 핀으로 입력되는 펄스의 시간을 측정하는 함수
  // pulseIn(echo, HIGH): 펄스를 입력받을 아두이노 핀은 echo이고, 측정하려는 펄스의 형태는 HIGH이다. 즉, echo가 HIGH 펄스를 받을 때 까지 걸리는 시간을 측정한다. 
  distance = ((float) (duration*340) / 10000) / 2; // 초음파센서와 물체 사이의 거리(여기서는 유모차와 사람 사이의 거리)

  Serial.print("거리 : "); // serial을 통해 데이터를 보냄
  Serial.print(distance);
  Serial.println("cm"); // 데이터 출력 이후 엔터 한번 쳐주기(그냥 print에는 엔터 쳐주는 기능이 없음)
  

  if (distance > 50) // 초음파센서(즉 유모차)와 사람 사이의 거리가 50cm 보다 클 때
  {
    angle = 60; 
    angle_1 = 120;
    delay(500);
    servo1.write(angle_1); // servo1 모터 120도 회전
    servo2.write(angle); // servo2 모터 60도 회전
    // servo 모터를 회전시켜 모터의 날을 유모차 바퀴의 기어에 맞물린다. 이로써 제동을 구현한다. 

  }
  else // 유모차와 사람 사이의 거리가 50cm 이하일 때
{
    angle = 90;
    delay(500);
    servo1.write(angle); 
    servo2.write(angle);
    // 두 개의 서보모터를 90도로 회전(초기상태) -> 이로써 제동을 푼다.  
  }


}


  
