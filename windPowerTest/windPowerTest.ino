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

#define COLOR_R 0
#define COLOR_G 0
#define COLOR_B 255

#define RESISTANCE_PIN A0
#define PHOTO_SENSOR_PIN 2
#define MOTOR_PIN 4

rgb_lcd lcd;
Servo esc;

/* Variable for resistance */
int resistanceVoltage = 0;
int resistanceValue = 0;
int resistanceValueForPrint = 0;

/* Variable for photosensor */
unsigned long mainTimer;
int windSpeedState = true;
int photoSensorCounter = 0;
float photoSensorCounterForMinute = 0.0;
float meterPerSecond = 0.0;
float circumference = 0.0;

void setup()
{
  Serial.begin(9600);

  /* LCD setup part */
  lcd.begin(16, 2); // set up the LCD's number of columns and rows:
  lcd.setRGB(COLOR_R, COLOR_G, COLOR_B);

  /* Motor setup part */
  esc.attach(MOTOR_PIN);

  /* Photo Sensor setup part */
  pinMode(PHOTO_SENSOR_PIN, INPUT);
  digitalWrite(PHOTO_SENSOR_PIN, HIGH);
}

void loop()
{
  motorControl();
  motorPowerPrint();
  readWindSpeed();
  windPowerPrint();
}

void motorControl()
{
  resistanceVoltage = analogRead(RESISTANCE_PIN);                // bring voltage from potentiometer
  resistanceValue = map(resistanceVoltage, 0, 1023, 65, 165);    // convert signal from 65 to 165 ( reason why is motor responds from 66 - 170 )
  esc.write(resistanceValue);                                    // use mapped value
  resistanceValueForPrint = resistanceValue - 65;                // to make value as 0 - 100
}

void readWindSpeed()
{
  mainTimer = millis() + 10000;

  // run while loop for 1s
  while (millis() < mainTimer)
  {
    delay(1); // 바운싱 제거를 위한 1ms 지연
    if ((windSpeedState == true) && !digitalRead(PHOTO_SENSOR_PIN)) {
      photoSensorCounter++;
      windSpeedState = false;
    }
    else if ((windSpeedState == false) && digitalRead(PHOTO_SENSOR_PIN)) {
      windSpeedState = true;
    }
  }
}

void motorPowerPrint()
{
  /* Motorpower print part */
  lcd.setCursor(0, 0);
  lcd.print("Motorpower : ");

  if (resistanceValueForPrint < 10) {
    lcd.print("00");
    lcd.print(resistanceValueForPrint);
  }
  else if (resistanceValueForPrint < 100) {
    lcd.print("0");
    lcd.print(resistanceValueForPrint);
  }
  else {
    lcd.print(resistanceValueForPrint);
  }
}

void windPowerPrint()
{
  lcd.setCursor(0, 1);

  // 2 x 22/7 (phi) x radian
  // example : the wheel radian is 10 cm. so the circumference is 2 * (22 / 7) * 10 = 62.86 cm.
  circumference = (2.0 * 22.0 / 7.0 * 0.0333);  // 3.33cm to 0.0333 meter
  photoSensorCounterForMinute = (6 * photoSensorCounter); // RPM for minute not 10 seconds
  // if the RPM number is 350 which is the length is 62.86 * 350/minute = 22000 cm or 220 m/minute = 220 m/ 60 second = 3.67 m/s.
  meterPerSecond = ( circumference * ( photoSensorCounterForMinute / 60.0) );

  lcd.print(photoSensorCounter);
  lcd.print(" times ");
  lcd.print(meterPerSecond);
  lcd.print(" m/s");
  photoSensorCounter = 0;
}


/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
