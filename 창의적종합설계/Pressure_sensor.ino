void setup() {
  // put your setup code here, to run once:

// 1번만 실행
int FSRReading = analogRead(FSR_pin); // 압력센서의 센서 값을 아날로그로 읽어 FSRReading 변수에 저장.
int FSRReading_1 = analogRead(FSR_pin_1);
int FSRReading_2 = analogRead(FSR_pin_2);
int FSRReading_3 = analogRead(FSR_pin_3);
int FSRReading_4 = analogRead(FSR_pin_4);
int FSRReading_5 = analogRead(FSR_pin_5);
int FSRReading_6 = analogRead(FSR_pin_6);
int FSRReading_7 = analogRead(FSR_pin_7);
int FSRReading_8 = analogRead(FSR_pin_8);

Serial.begin(9600);
Serial.println(FSRReading); // 센서 값 출력
Serial.println(FSRReading_1);
Serial.println(FSRReading_2);
Serial.println(FSRReading_3);
Serial.println(FSRReading_4);
Serial.println(FSRReading_5);
Serial.println(FSRReading_6);
Serial.println(FSRReading_7);
Serial.println(FSRReading_8);


}

void loop() {
  // put your main code here, to run repeatedly:

}
