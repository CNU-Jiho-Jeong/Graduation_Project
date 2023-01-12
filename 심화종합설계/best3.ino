#include "LedControl.h" // ledcontrol 라이브러리 불러오기

// const: 변경이 불가능한 상수로 인식하게 함  
const int ledA = 13; // 차량 신호 (적색) - 아두이노 13번 핀
const int ledB = 12; // 차량 신호 (황색) - 아두이노 12번 핀

const int ledA1 = 30; // 보행자 신호 (적색) - 아두이노 30번 핀
const int ledA2 = 31; // 보행자 신호 (적색) - 아두이노 31번 핀

const int ledC = 32; // 보행자 신호 (청색) - 아두이노 32번 핀
const int ledC1 = 33; // 보행자 신호 (청색) - 아두이노 33번 핀

int trig = 22, echo = 23; // 초음파센서1: 발사부분(trig) - 아두이노 22번 핀, 받는부분(echo) - 아두이노 23번 핀 
int trig1 = 24, echo1 = 25; // 초음파센서2: 발사부분(trig) - 아두이노 24번 핀, 받는부분(echo) - 아두이노 25번 핀

// 7-segment
byte ledPin[8]={2, 3, 4, 5, 6, 7, 8, 9}; // 아두이노 핀 번호(2~9번까지)
byte code[10]={63,6,91,79,102,109,124,7,127,103}; // code numbers 0 to 9. 왜 0부터 9까지가 63,6,91..103인지는 afficher함수 참고.
int unite=0; // 타이머의 1의 자리
int dizaine=0; // 타이머의 10의자리
int duree; // 타이머(1초 경과마다 1씩 내려감)

// 32x8 LED Matrix Panel(Red)
LedControl lc=LedControl(27,28,26,4); // lc는 이름임. 바뀌어도 됨. 
// LedControl lc=LedControl(Data In 핀번호, CLOCK 핀번호, LOAD 핀번호, 직렬로 연결된 Led 드라이버 모듈의 개수)
// Data In == DIN, 데이터 입력 핀
// CLOCK == CLK, 클럭 신호 입력 핀       
// LOAD == CS, 여러 개의 도트 매트릭스 모듈 사용 시 칩 선택 핀 
// 여기서 직렬로 연결된 Led 드라이버 모듈의 개수는 4개이다(32X8 LED Matrix Panel)
int num; 
// 32X8 LED Matrix Panel은 0,1,2,3번째 매트릭스, 총 4개로 이루어져 있다. 
// num은 이 0~3번을 위한 변수이다. 

// 32x8 LED Matrix Panel(Green)
LedControl lc1=LedControl(34,35,36,4);
int num1;




void setup() {
  // put your setup code here, to run once:

// 출력 및 입력 설정. led는 출력. 초음파센서의 echo는 입력.
pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
pinMode(trig1, OUTPUT);
pinMode(echo1, INPUT);

pinMode(ledA, OUTPUT);
pinMode(ledA1, OUTPUT);
pinMode(ledA2, OUTPUT);
pinMode(ledB, OUTPUT);
pinMode(ledC, OUTPUT);
pinMode(ledC1, OUTPUT);

for (int i=0; i<=8; i++) // 2~9번까지 핀번호 다 출력으로 설정 -> 7segment를 출력으로 설정
  {
pinMode(ledPin[i], OUTPUT);
  }

for(num=0; num<4; num++) {
  lc.shutdown(num,false); 
  // name.shutdown(정수인 address, true or false): 절전모드를 제어하는 함수
  // 정수인 address는 연결된 매트릭스의 주소(0번째인지, 1번째인지.....3번째인지)
  // true를 주면 절전모드, false를 주면 절전모드 해제
  lc.setIntensity(num,10);
  // name.setIntensity(정수인 address, 정수인 밝기): 밝기 조절 함수
  // 정수인 address는 연결된 매트릭스의 주소(0번째인지, 1번째인지.....3번째인지)
  // 정수인 밝기는 0~15로 조절 가능. 커질 수록 밝다. 
  // 붉은색 led matrix의 밝기를 청색 led matrix보다 크게 했다. 청색이 너무 밝으면 적색이 묻혀버렸기 때문이다.   
  lc.clearDisplay(num); 
  //name.clearDisplay(정수인 address)
  // 지정된 주소의 매트릭스 초기화
 }


for(num1=0; num1<4; num1++) {
  lc1.shutdown(num1,false);
  lc1.setIntensity(num1,3); // 청색 LED의 밝기를 적색에 비해 많이 줄였다. 
  lc1.clearDisplay(num1);
 }


}

void afficher(int x) // Procedure coding the number in 7 segments
{
byte chiffre=x;
// byte: 0 ~ 255의 8비트 부호없는 숫자값을 가진다. ex) byte b = B10010; -> B10010에서 'B'는 데이터가 이진형식이라는걸 나타낸다.
byte segment=0; // segment 초기 값은 0 
for (int i=2; i<9; i++) // i는 2부터 8까지
  {
segment=chiffre&00000001; // segment == 1 or segment == 0
digitalWrite(i,segment); 
// digitalWrite(핀번호, 값) -> 여기서 값이 0이면 LOW, 1이면 HIGH -> segment가 1이면 HIGH, 0이면 LOW. ->  2~8번 핀을 상황에 따라 HIGH 아니면 LOW로 만들어준다.  
chiffre=chiffre>>1; // chiffre를 2로 나눈다. 
// >>: 비트열을 오른쪽으로 1칸씩 이동시키면서 정수값을 2로 나눈다.
// <<: 비트열을 왼쪽으로 1칸씩 이동시키면서 정수값은 2배로 만든다. 
  }
}


void matrix() // matrix()함수 정의. matrix()함수는 보행자 정지선 패턴(=)을 나타내 주도록 하는 함수이다. 
{
  byte m[8]={ // 첫번째 매트릭스 패턴. m[0]은 아래 8개 행들 중 첫번째 행(B00001111), m[1]은 두 번째 행(B00001111).....을 의미한다. 
    B00001111,
    B00001111,
    B00001111,
    B00001111,
    B00001111,
    B00001111,
    B00001111,
    B00001111
  }; //byte m[8]에서의 8은 '8번째'를 의미하는게 아니고, 8개의 행을 지녔다는 것을 의미한다. 
  byte t[8]={ // 두번째 매트릭스 패턴
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  };
  byte r[8]={
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000
  };
  byte x[8]={
    B11110000,
    B11110000,
    B11110000,
    B11110000,
    B11110000,
    B11110000,
    B11110000,
    B11110000
  };

for(int j=0; j<8; j++){
  lc.setRow(0,j,m[j]); 
  // setRow(n번째 매트릭스, 출력할 행, 출력할 값). 
  // 출력할 값(세번째 인수): 한 행은 8개의 LED로 구성되어있으므로 on/off 상태값 8개를 한번에 제공해야 한다. ex) B11110000
  lc.setRow(1,j,t[j]);
  lc.setRow(2,j,r[j]);
  lc.setRow(3,j,x[j]);
}

}

void matrix1() // matrix1()함수는 차량 정지선 패턴(|)을 나타내 주도록 하는 함수이다. 
{
  byte m[8]={
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
  byte t[8]={
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
  byte r[8]={
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
  byte x[8]={
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B00000000,
    B11111111,
    B11111111
  };
for(int j=0; j<8; j++){
  lc.setRow(0,j,m[j]);
  lc.setRow(1,j,t[j]);
  lc.setRow(2,j,r[j]);
  lc.setRow(3,j,x[j]); 
}

}


void matrix2() // matrix2()함수는 보행자가 횡단 보도를 건너도록 청색 화살표 패턴을 나타내 주도록 하는 함수이다.
{
  
byte m[8]={
    B00001111,
    B00011111,
    B00111111,
    B01111111,
    B01111111,
    B00111111,
    B00011111,
    B00001111
  };
  byte t[8]={
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111
  };
  byte r[8]={
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111,
    B11111111
  };
  byte x[8]={
    B11110000,
    B11111000,
    B11111100,
    B11111110,
    B11111110,
    B11111100,
    B11111000,
    B11110000
  };

for(int j=0; j<8; j++){
  lc1.setRow(0,j,m[j]);
  lc1.setRow(1,j,t[j]);
  lc1.setRow(2,j,r[j]);
  lc1.setRow(3,j,x[j]);
}

}

void loop() {
  // put your main code here, to run repeatedly:
float duration, distance;  // 초음파센서1의 시간, 거리 변수 정의 (실수형) 
float duration1, distance1; // 초음파센서2의 시간, 거리 변수 정의 (실수형) 


digitalWrite(trig, HIGH);
delay(1);
digitalWrite(trig, LOW);
duration = pulseIn(echo, HIGH); // echo에 펄스형태 신호가 들어오기까지 걸린 시간
distance = ((float)(duration * 340) / 10000) / 2; // 초음파센서1과 물체(사람)사이의 거리 계산

digitalWrite(trig1, HIGH);
delay(1);
digitalWrite(trig1, LOW);
duration1 = pulseIn(echo1, HIGH);
distance1 = ((float)(duration1 * 340) / 10000) / 2; // 초음파센서2과 물체(사람)사이의 거리 계산


if (distance < 10) // 거리가 10보다 작으면 (즉 사람이 감지가 되면)
{

// 바로 신호체계를 작동시키지 않고 황색 점멸을 잠깐 유지한다.  
digitalWrite(ledB, HIGH);
delay(1000);
digitalWrite(ledB,LOW);
delay(1000); 



for(int i = 0; i<=37 ; i++) {
duree= 37 - (millis() / 1000);// 37, 36, 35, 34.....
dizaine=duree/10; // duree의 10의 자리
unite=duree%10; // duree의 1의 자리

if (duree<=0) 
{
// duree==0이 되면 신호체계가 원래대로 돌아온다.  


digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(7,LOW);
digitalWrite(8,LOW);
digitalWrite(ledA, LOW);
digitalWrite(ledA1, LOW);
digitalWrite(ledA2, LOW);
digitalWrite(ledC, LOW);
digitalWrite(ledC1, LOW);
for(int k = 5; k>0 ; k--) {
  digitalWrite(ledB, HIGH);
  delay(1000);
  digitalWrite(ledB,LOW);
  delay(1000); 

}

}


while(duree>22) { 
  // duree>22 일 때, 즉 아두이노 실행 후 경과시간 15초를 찍기 전에: 황색불은 점멸 상태에서 on 상태로 변경. 보행자 바닥 신호등은 적색 불 on. Dot matrix는 보행자 정지 패턴(=) 조사  
  
  digitalWrite(ledA, LOW);
  digitalWrite(ledC, LOW);
  digitalWrite(ledC1, LOW);
  
  // 황색불은 점멸 상태에서 on 상태로 변경
  digitalWrite(ledB, HIGH);
  
  // 보행자 바닥 신호등은 적색 불 on (두 개)
  digitalWrite(ledA1, HIGH);
  digitalWrite(ledA2, HIGH);

  // Dot matrix는 보행자 정지 패턴(=) 조사 
  matrix();

  // 7-segment는 꺼져있는 상태
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);

  // 아두이노 실행 후 경과시간이 15초 이상이 되면
  if(millis()/1000>=15) 
  {
    matrix1(); // 차량 정지선 패턴(|) 조사
    break;
  } 
}



digitalWrite(9,LOW); // selection of units on display. 9번핀 LOW 시켜서 7segment의 일의자리를 켜주고, 그 다음 9번핀을 HIGH 시켜서 7segment의 10의 자리를 켜준다.  
afficher(code[unite]); // show units
// byte code[10] = {63,6,91,79,102,109,124,7,127,103}; -> 0부터 9까지 순서대로(0은 63, 1은 6...)
// afficher(code[unite]): code[unite]는 unite를 나타내는 값(63,6,91...등등)이 된다. 그 값을 afficher 함수에 넣으면 세그먼트의 일의 자리에 unite가 표시된다. 
// unite를 segment에 표시하기 위해 2~8번 핀 중 켜야하는 핀 번호가 있는데, 이를 위해 63, 6, 91 등의 숫자가 정해진 것이다.  
delay(10);
digitalWrite(9,HIGH); // selection of tens on display
afficher(code[dizaine]); //show tens 
// afficher(code[dizaine]): code[dizane]은 dizane을 나타내는 값이 된다. 그 값을 afficher 함수에 넣으면 세그먼트의 십의 자리에 dizane이 표시된다. 
// dizane을 segment에 표시하기 위해 2~8번 핀 중 켜야하는 핀 번호가 있는데, 이를 위해 63, 6, 91 등의 숫자가 정해진 것이다.  
delay(10);

// 황색불 꺼짐
digitalWrite(ledB, LOW);

// 차량 신호등 빨간색 on
digitalWrite(ledA, HIGH);

// 보행자 바닥 신호등 빨간색 off
digitalWrite(ledA1, LOW);
digitalWrite(ledA2, LOW);

// 보행자 바닥 신호등 초록색 on
digitalWrite(ledC, HIGH);
digitalWrite(ledC1, HIGH);

// 청색 화살표 패턴 횡단보도에 조사 
matrix2();

// 7-segment의 시간이 0~10초일때
while(duree<=10 && duree>=0)
{
for(int i = 0; i<=37 ; i++) {
duree= 37 - (millis() / 1000);// 10, 9, 8, 7....
dizaine=duree/10; // duree의 십의자리. duree가 1의 자리가 되면 0이 되어 7segment의 십의자리에는 0이 표시된다. 
unite=duree%10; // duree의 일의자리.

 //  7-segment의 시간이 0이 되면 원래의 신호체계로!
if (duree<=0)
{

digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(7,LOW);
digitalWrite(8,LOW);
digitalWrite(ledA, LOW);
digitalWrite(ledC, LOW);
digitalWrite(ledC1, LOW);

for(num1=0; num1<4; num1++)
  {
  lc1.clearDisplay(num1);
  }

for(num=0; num<4; num++)
  {
  lc.clearDisplay(num);
  }

for(int k = 5; k>0 ; k--) {
  digitalWrite(ledB, HIGH);
  delay(1000);
  digitalWrite(ledB,LOW);
  delay(1000); 
}

}

digitalWrite(9,LOW); // selection of units on display
afficher(code[unite]); // show units
delay(10);
digitalWrite(9,HIGH); // selection of tens on display
afficher(code[dizaine]); //show tens 
delay(10);
digitalWrite(ledB, LOW);
digitalWrite(ledA, HIGH);
digitalWrite(ledC, HIGH);
digitalWrite(ledC1, HIGH);
 
if(millis()/1000 >= 27 && (millis()/1000)%2 ==0) { // segment의 시간이 10초 이하일 때, 그 시간이 홀수이면 패턴 유지
  matrix2();
}

  else if(millis()/1000 >= 27 && (millis()/1000)%2==1) // segment의 시간이 10초 이하일 때, 그 시간이 짝수이면 패턴 끄기 -> 불빛 깜빡거림 구현 -> 시간이 얼마 남지 않았으니 얼른 건너라! 
  {
    for(num1=0;num1<4;num1++) 
    {
    lc1.clearDisplay(num1);
    } 
  }
  
if(millis()/1000>=37) // 시간이 다 되면 dot matrix off
{
  for(num1=0; num1<4; num1++)
  {
  lc1.clearDisplay(num1);
  }
  for(num=0; num<4; num++)
  {
  lc.clearDisplay(num);
  }
    break; // 초음파 반복문 탈출
}
  
}


}
  
}

}

else if (distance1 < 10) // 초음파 2에서 사람이 감지되면 (위와 동일하게)
{

digitalWrite(ledB, HIGH);
delay(1000);
digitalWrite(ledB,LOW);
delay(1000); 

for(int i = 0; i<=37 ; i--) {
duree= 37 - (millis() / 1000);// stopwatch
dizaine=duree/10; // Extraction of tens
unite=duree%10; // Extraction of units

if (duree<=0) 
{


digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(7,LOW);
digitalWrite(8,LOW);
digitalWrite(ledA, LOW);
digitalWrite(ledA1, LOW);
digitalWrite(ledA2, LOW);
digitalWrite(ledC, LOW);
digitalWrite(ledC1, LOW);
for(int k = 5; k>0 ; k--) {
  digitalWrite(ledB, HIGH);
  delay(1000);
  digitalWrite(ledB,LOW);
  delay(1000); 

}

}

while(duree>22) {
  
  digitalWrite(ledA, LOW);
  digitalWrite(ledB, HIGH);
  digitalWrite(ledC, LOW);
  digitalWrite(ledC1, LOW);
  digitalWrite(ledA1, HIGH);
  digitalWrite(ledA2, HIGH);
  matrix();
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  digitalWrite(5,LOW);
  digitalWrite(6,LOW);
  digitalWrite(7,LOW);
  digitalWrite(8,LOW);
  if(millis()/1000>=15) 
  {
    matrix1();
    break;
  } 
}

digitalWrite(9,LOW); // selection of units on display
afficher(code[unite]); // show units
delay(10);
digitalWrite(9,HIGH); // selection of tens on display
afficher(code[dizaine]); //show tens 
delay(10);
digitalWrite(ledB, LOW);
digitalWrite(ledA, HIGH);
digitalWrite(ledA1, LOW);
digitalWrite(ledA2, LOW);
digitalWrite(ledC, HIGH);
digitalWrite(ledC1, HIGH);
matrix2();

while(duree<=10 && duree>=0)
{
for(int i = 0; i<=37 ; i--) {
duree= 37 - (millis() / 1000);// stopwatch
dizaine=duree/10; // Extraction of tens
unite=duree%10; // Extraction of units

if (duree<=0) 
{

digitalWrite(2,LOW);
digitalWrite(3,LOW);
digitalWrite(4,LOW);
digitalWrite(5,LOW);
digitalWrite(6,LOW);
digitalWrite(7,LOW);
digitalWrite(8,LOW);
digitalWrite(ledA, LOW);
digitalWrite(ledC, LOW);
digitalWrite(ledC1, LOW);

for(num1=0; num1<4; num1++)
  {
  lc1.clearDisplay(num1);
  }

for(num=0; num<4; num++)
  {
  lc.clearDisplay(num);
  }

for(int k = 5; k>0 ; k--) {
  digitalWrite(ledB, HIGH);
  delay(1000);
  digitalWrite(ledB,LOW);
  delay(1000); 
}

}

digitalWrite(9,LOW); // selection of units on display
afficher(code[unite]); // show units
delay(10);
digitalWrite(9,HIGH); // selection of tens on display
afficher(code[dizaine]); //show tens 
delay(10);
digitalWrite(ledB, LOW);
digitalWrite(ledA, HIGH);
digitalWrite(ledC, HIGH);
digitalWrite(ledC1, HIGH);
 
if(millis()/1000 >= 27 && (millis()/1000)%2 ==0) {
  matrix2();
}

  else if(millis()/1000 >= 27 && (millis()/1000)%2==1) 
{
    for(num1=0;num1<4;num1++) 
    {
    lc1.clearDisplay(num1);
    } 
}
  
if(millis()/1000>=37) 
{
  for(num1=0; num1<4; num1++)
  {
  lc1.clearDisplay(num1);
  }
  for(num=0; num<4; num++)
  {
  lc.clearDisplay(num);
  }
    break;
}
  
}


}
  
}

}

// 사람이 감지 안되면 계속 황색불 점멸 상태 
else
{
 digitalWrite(ledB, HIGH);
 delay(1000);
 digitalWrite(ledB,LOW);
 delay(1000); 
}


}
