/*****************************************************************************/
//  Function:    Get the accelemeter of the x/y/z axis. 
//  Hardware:    Grove - 3-Axis Digital Accelerometer(±1.5g)
//  Arduino IDE: Arduino-1.0
//  Author:  Frankie.Chu    
//  Date:    Jan 10,2013
//  Version: v0.9b
//  by www.seeedstudio.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
//
/*******************************************************************************/

#include <Wire.h>
#include <Arduino.h>
#include <Wire.h>         // this #include still required because the RTClib depends on it
#include "RTClib.h"
#include "MMA7660.h"
MMA7660 accelemeter;

#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

RTC_Millis rtc;


float az_count = 0;
float az_save = 0;
float az_last = 0;
float az_sum = 0;
float threshold = 0.9;     //  A m/s2
float t = 5;               //  the duration of a loop ms
float point = 5;           //  wave filter variable
float p_wave = 0;
float t_wave = 0;
float t_sum = 0;
float f_max = 6;           //  f max Hz
float f_min = 3;         //  f min Hz
float T_max = 1000/f_min;  //  T max ms
float T_min = 1000/f_max;  //  T min ms

void setup()
{
  
  accelemeter.init();  
  Serial.begin(9600);
  
  Serial.println("acceleration    Amount of fidget   Duration of fidget      Time                Running seconds");
  rtc.begin(DateTime(F(__DATE__), F(__TIME__)));
 
}
void loop()
{
  int8_t x;
  int8_t y;
  int8_t z;
  unsigned long time;
  int8_t z_mean = 1;
  float ax,ay,az;
  accelemeter.getXYZ(&x,&y,&z);
  accelemeter.getAcceleration(&ax,&ay,&az);
  
  az = sqrt(az*az + ax*ax + ay*ay) - z_mean;
   // Serial.println(String(az));
  //Serial.println("**********************");
  //Serial.println(az);
  int8_t i = 0;
  while(i < point){
    // do something repetitive 8 times
    az_sum += az;
    i++;
    continue;
  //Serial.println("----------------------");
  }
  // 第1／2周期
  if(az > threshold){
    az_save = az;
    az = 1;
    if(az == az_last){
      // do nothing
      p_wave++;
    }else{
      t_wave = p_wave * point * t;  //ms
      if((t_wave < T_max/2) && (t_wave > T_min/2)){
        t_sum += t_wave;
      }else{
        // do nothing
      }
      
      
      az_count += 0.5;
      az_last = az;
      t_wave = 0;
      p_wave = 0;
    Serial.print(az_save);
    Serial.print(",         ");
    DateTime now = rtc.now();
    String sensorVal = String(az_count) + String(t_sum/1000) ;
    //Serial.print("Count--");
    Serial.print(String(az_count));
    Serial.print(",                ");
    //Serial.print("---------Duration--");
    Serial.print(String(t_sum/1000));
    Serial.print(",                   ");
    //Serial.print("---------Time--");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(",   ");
    time = millis();            // 印出 millis() 的數值
    Serial.print(time/1000);

    
    //Serial.println();
    Serial.print(",");
    
    Serial.println("");
    }
  }
  if(az < threshold){
    az_save = az;
    az = 0;
    if(az == az_last){
      // do nothing
      p_wave++;
    }else{
      t_wave = p_wave * point * t;  //ms
      if((t_wave < T_max/2) && (t_wave > T_min/2)){t_sum += t_wave;
        t_sum += t_wave;
      }else{
        // do nothing
      }
      //Serial.println(t_sum);
      az_count += 0.5;
      az_last = az;
      t_wave = 0;
      p_wave = 0;
      
  
      
    Serial.print(az_save);
    Serial.print(",         ");
    DateTime now = rtc.now();
    String sensorVal = String(az_count) + String(t_sum/1000) ;
    //Serial.print("Count--");
    Serial.print(String(az_count));
    Serial.print(",                ");
    //Serial.print("---------Duration--");
    Serial.print(String(t_sum/1000));
    Serial.print(",                   ");
    //Serial.print("---------Time--");
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(' ');
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.print(",   ");
    time = millis();            // 印出 millis() 的數值
    Serial.print(time/1000);
    Serial.print(",");
    Serial.println("");

    //Serial.print(String(az_count));
    }
    
  }
    
  // shaking time
  //Serial.println(t_sum/1000);
  // shaking counts
  delay(t);
}


