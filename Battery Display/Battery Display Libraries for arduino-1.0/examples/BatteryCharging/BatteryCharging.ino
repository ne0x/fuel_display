/***********************************************************/
//Demo for the the Battery Display by Catalex
//Hardware: Battery Display *1
//Board:  Arduino UNO R3
//IDE:	  Arduino-1.0
//Function:  Simulate charging display of a battery.
//Store: http://www.aliexpress.com/store/1199788
//          http://www.dx.com/
/***************************************************************/

#include "TM1651.h"
#define CLK 3//pins definitions for TM1651 and can be changed to other ports       
#define DIO 2
TM1651 batteryDisplay(CLK,DIO);
void setup()
{
  batteryDisplay.init();
  batteryDisplay.set(BRIGHTEST);//BRIGHT_TYPICAL = 2,BRIGHT_DARKEST = 0,BRIGHTEST = 7;
  batteryDisplay.frame(FRAME_ON);//light the frame of the battery display
}
void loop()
{
  charging();
}

void charging()
{
  for(uint8_t level = 0; level < 6; level ++)
  {
    batteryDisplay.displayLevel(level);
	delay(500);
  }
}
