/***********************************************************/
//Demo for the the Battery Display by Catalex
//Hardware: Battery Display *1
//Board:  Arduino UNO R3
//IDE:	  Arduino-1.0
//Function:  Simulate charging display of a battery.
/***************************************************************/

#include <math.h>
#include "TM1651.h"
#define CLK 3//pins definitions for TM1651 and can be changed to other ports       
#define DIO 2

TM1651 batteryDisplay(CLK,DIO);

int analogPin = 1;
int raw = 0;
int Vin = 5;
float Vout = 0;
float R1 = 10000;
int R2 = 0;
float buffer = 0;
int R;

int fullR = 7;
int emptyR = 330;
float oneLevel = emptyR / 5;

void setup()
{
  batteryDisplay.init();
  batteryDisplay.set(3);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  batteryDisplay.frame(FRAME_ON);//light the frame of the battery display
  
  //Serial.begin(115200);
}

void loop()
{
  R = ohmmeter();
  fuellevel(R);
  delay(1000);
}

void fuellevel(int R)
{
  if(R > emptyR) 
  {
    blinkEmpty();
  }
  else if(R < fullR) 
  {
    batteryDisplay.displayLevel(5);
    if(R < 0)
    {
      blinkShort();
    }
  }
  else 
  {
    int l = (int) round(R / oneLevel);
    if(l > 5)
    {
      l = 5;
    }
    uint8_t level = 5 - l;
    batteryDisplay.displayLevel(level);
  }
}

int ohmmeter()
{
  raw= analogRead(analogPin);
  if(raw)
  {
    buffer = raw * Vin;
    Vout = (buffer)/1024.0;
    buffer = 1/((Vin/Vout)-1);
    R2 = (int) round(R1 * buffer / 1000);
    
    //Serial.print("Vout: ");
    //Serial.println(Vout);
    //Serial.print("R2: ");
    //Serial.println(R2);
  }
  else
  {
    R2 = -1;
  }
  
  return R2;
}

void blinkEmpty()
{
  batteryDisplay.displayLevel(0);
  batteryDisplay.frame(FRAME_OFF);
  delay(400);
  batteryDisplay.frame(FRAME_ON);
  delay(400);
  batteryDisplay.frame(FRAME_OFF);
  delay(400);
  batteryDisplay.frame(FRAME_ON);
}

void blinkShort() //if short circuit
{
  batteryDisplay.frame(FRAME_OFF);
  delay(400);
  batteryDisplay.frame(FRAME_ON);
  delay(400);
  batteryDisplay.frame(FRAME_OFF);
  delay(400);
  batteryDisplay.frame(FRAME_ON);
}
