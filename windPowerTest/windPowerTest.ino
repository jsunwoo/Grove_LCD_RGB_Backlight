/*
  Hello World.ino
  2013 Copyright (c) Seeed Technology Inc.  All right reserved.

  Author:Loovee
  2013-9-18

  Grove - Serial LCD RGB Backlight demo.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <Wire.h>
#include <Servo.h>
#include "rgb_lcd.h"

rgb_lcd lcd;
Servo esc;

const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

int sourceVoltage = 5;
int resistancePin = A0;
int resistanceVoltage = 0;
int resistanceValue = 0;
int printValue = 0;


//////wind speed
unsigned long mainTimer;
float windSpeedResult = 0.0;
int windSpeedState = true; //스위치의 상태를 저장한다.
int windSpeedCounter = 0; //스위치가 몇 번 눌렸는지 알 수 있는 변수
int printCounter = 0;
float ms = 0.0;
float ms1 = 0.0;
float circumference = 0.0;
//////
int windSpeedPin = 2;
int windSpeed = 0;


void setup()
{
  Serial.begin(9600);

  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.setRGB(colorR, colorG, colorB);
//  lcd.print("Resistance Value"); // Print a message to the LCD.
  esc.attach(4);    // Digital 4th Pin



  pinMode(windSpeedPin, INPUT); //7번 핀을 스위치의 용도로 쓰기
  digitalWrite(windSpeedPin, HIGH); //위한 설정 (풀업 저항 가동)



  delay(1000);
}

void loop()
{
  motorControl();
  readWindSpeed();
  finalPrint();

}

void motorControl() {
  // voltage is going to be 0~1023
  resistanceVoltage = analogRead(resistancePin);
  resistanceValue = map(resistanceVoltage, 0, 1023, 65, 165);

  esc.write(resistanceValue);   // 모터출력
  printValue = resistanceValue - 65;  // 0에서 100까지
}




float readWindSpeed() {
  mainTimer = millis() + 100; // 0.1초를 주기로 반복
  while (millis() < mainTimer) {
    delay(1); //바운싱 제거를 위한 1ms 지연
    if ((windSpeedState == true) && !digitalRead(windSpeedPin)) {
      windSpeedCounter++;
      windSpeedState = false;
    }
    else if ((windSpeedState == false) && digitalRead(windSpeedPin)) {
      windSpeedState = true;
    }
  }

  
    windSpeedResult = (6 * windSpeedCounter);  //rpm 으로 보정

    circumference = (2.0 * 22.0 / 7.0 * 0.0333) ; // 3.4cm날개 반경을 보정 m로 보정
    // 2 x 22/7 (phi) x radian) of the wheel
    // example : the wheel radiant is 10 cm. so the circumference is 2*22/7*10 = 62.86 cm.
    // if the RPM number is 350 which is the length is 62.86*350/minute = 22000 cm or 220 m/minute = 220 m/ 60 second = 3.67 m/s.
    ms1 = ((windSpeedResult * circumference) / 60.0 );

    //ms = ((3.1415 * 0.03) * windSpeed ) / 60.0 ;  // circumfrance of the circle in meters : pi * the diameter(반경)

  



  printCounter++;
//  Serial.print(printCounter);
//  Serial.print("  ");

  if ( (printCounter % 10) == 0) {  // 10초에 한번씩
    Serial.print("windSpeed : ");
    Serial.print(ms1); //스위칭 횟수를 출력한다.
    Serial.println("m/s");

    lcd.setCursor(0, 1);
    lcd.print("Speed : ");
    lcd.print(ms1);
    lcd.print(" m/s");

    windSpeedCounter = 0; //스위칭 횟수 초기화, 다시 0부터 센다

  }

  return ms1;
}

void finalPrint() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 0);

  // print the number of seconds since reset:
  // 앞자리 숫자가 이상해서
  if (printValue == 0) {
    lcd.print("Motorpower : ");
    lcd.print("000");
  }
  else if (printValue < 10) {
    lcd.print("Motorpower : ");
    lcd.print("00");
    lcd.print(printValue);
  }
  else if (printValue < 100) {
    lcd.print("Motorpower : ");
    lcd.print("0");
    lcd.print(printValue);
  }
  else {
    lcd.print("Motorpower : ");
    lcd.print(printValue);
  }

  //  delay(100);
}




/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
