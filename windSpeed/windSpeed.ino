#include <Wire.h>

//////wind speed
unsigned long mainTimer;
float windSpeedResult = 0.0;
int windSpeedState = true; //스위치의 상태를 저장한다.
int windSpeedCounter = 0; //스위치가 몇 번 눌렸는지 알 수 있는 변수
float ms = 0.0;
float ms1 = 0.0;
float circumference = 0.0;
//////
int windSpeedPin = 2;
int windSpeed = 0;

void setup() {
  Serial.begin(9600);
  pinMode(windSpeedPin, INPUT); //7번 핀을 스위치의 용도로 쓰기
  digitalWrite(windSpeedPin, HIGH); //위한 설정 (풀업 저항 가동)
}


void loop() {
  readWindSpeed();
  delay(1000);
}

float readWindSpeed(){
  mainTimer = millis() + 1000; //10초를 주기로 반복
  while(millis() < mainTimer){
    delay(1); //바운싱 제거를 위한 1ms 지연
    if((windSpeedState == true) && !digitalRead(windSpeedPin)){
      windSpeedCounter++;
      windSpeedState = false;
    }
    else if((windSpeedState == false) && digitalRead(windSpeedPin)){
      windSpeedState = true;
    }
  }
  windSpeedResult = (6 * windSpeedCounter);  //rpm 으로 보정
  
  circumference = (2.0*22.0/7.0* 0.0333) ; // 3.4cm날개 반경을 보정 m로 보정  
      // 2 x 22/7 (phi) x radian) of the wheel
      // example : the wheel radiant is 10 cm. so the circumference is 2*22/7*10 = 62.86 cm.
      // if the RPM number is 350 which is the length is 62.86*350/minute = 22000 cm or 220 m/minute = 220 m/ 60 second = 3.67 m/s. 
  ms1 = ((windSpeedResult * circumference) / 60.0 );
  
  //ms = ((3.1415 * 0.03) * windSpeed ) / 60.0 ;  // circumfrance of the circle in meters : pi * the diameter(반경)
  Serial.print("windSpeed : ");
  Serial.print(ms1); //스위칭 횟수를 출력한다.
  Serial.println("m/s");
  windSpeedCounter = 0; //스위칭 횟수 초기화, 다시 0부터 센다
  return ms1;
}
