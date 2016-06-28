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
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 0;
const int colorG = 0;
const int colorB = 255;

//
int sourceVoltage = 5;
int resistancePin = A0;
float resistanceVoltage = 0;
int resistanceValue = 0;

void setup()
{
  //
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  lcd.setRGB(colorR, colorG, colorB);

  // Print a message to the LCD.
  lcd.print("Resistance Value");

  delay(1000);
}

void loop()
{
  // voltage is going to be 0~1023
  resistanceVoltage = (sourceVoltage / 1023.0) * analogRead(resistancePin);
  // reason why using minus instead of devide is for balance
  resistanceValue = ( (4.7 - resistanceVoltage) * 66 ) - 3;


  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);

  // print the number of seconds since reset:
  // 앞자리 숫자가 이상해서
  if (resistanceValue < 10) {
    lcd.print("00");
    lcd.print(resistanceValue);
    Serial.println(resistanceValue);
  }
  else if (resistanceValue < 100) {
    lcd.print("0");
    lcd.print(resistanceValue);
    Serial.println(resistanceValue);
  }
  else {
    lcd.print(resistanceValue);
    Serial.println(resistanceValue);
  }

  delay(100);
}

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
