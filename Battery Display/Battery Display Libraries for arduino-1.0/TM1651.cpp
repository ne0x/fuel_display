//  Author:Fred.Chu
//  Date:14 August, 2014
//
//  Applicable Module:
//                     Battery Display v1.0
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
//  Modified record:
//
/*******************************************************************************/
#include "TM1651.h"
#include <Arduino.h>
static int8_t LevelTab[] = {0x00,0x01,0x03,0x07,0x0f,0x1f,};//Level 0~5

TM1651::TM1651(uint8_t Clk, uint8_t Data)
{
  Clkpin = Clk;
  Datapin = Data;
  pinMode(Clkpin,OUTPUT);
  pinMode(Datapin,OUTPUT);
}

void TM1651::init()
{
  set(BRIGHT_TYPICAL);
  clearDisplay();
}

void TM1651::writeByte(int8_t wr_data)
{
  uint8_t i,count1;   
  for(i=0;i<8;i++)        //sent 8bit data
  {
    digitalWrite(Clkpin,LOW);      
    if(wr_data & 0x01)digitalWrite(Datapin,HIGH);//LSB first
    else digitalWrite(Datapin,LOW);
	delayMicroseconds(3);
    wr_data >>= 1;      
    digitalWrite(Clkpin,HIGH);
	delayMicroseconds(3);
      
  }  
  digitalWrite(Clkpin,LOW); //wait for the ACK
  digitalWrite(Datapin,HIGH);
  digitalWrite(Clkpin,HIGH);     
  pinMode(Datapin,INPUT);
  while(digitalRead(Datapin))    
  { 
    count1 +=1;
    if(count1 == 200)//
    {
     pinMode(Datapin,OUTPUT);
     digitalWrite(Datapin,LOW);
     count1 =0;
    }
    pinMode(Datapin,INPUT);
  }
  pinMode(Datapin,OUTPUT);
  
}
//send start signal to TM1651
void TM1651::start(void)
{
  digitalWrite(Clkpin,HIGH);//send start signal to TM1651
  digitalWrite(Datapin,HIGH);
  delayMicroseconds(2);
  digitalWrite(Datapin,LOW); 
  digitalWrite(Clkpin,LOW); 
} 
//End of transmission
void TM1651::stop(void)
{
  digitalWrite(Clkpin,LOW);
 // delayMicroseconds(2);
  digitalWrite(Datapin,LOW);
//  delayMicroseconds(2);
  digitalWrite(Clkpin,HIGH);
 // delayMicroseconds(2);
  digitalWrite(Datapin,HIGH); 
}
//******************************************
void TM1651::displayLevel(uint8_t Level)
{
  if(Level > 5)return;//Level should be 0~5
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_FIXED);//
  stop();           //
  start();          //
  writeByte(0xc0);//
  writeByte(LevelTab[Level]);//
  stop();            //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

void TM1651::frame(boolean FrameFlag)
{
  int8_t SegData;
  if (FrameFlag == 1) SegData = 0x40;
  else SegData = 0x00;
  start();          //start signal sent to TM1651 from MCU
  writeByte(ADDR_AUTO);//
  stop();           //
  start();          //
  writeByte(0xc1);//
  for(uint8_t i=0;i < 3;i ++)
  {
    writeByte(SegData);        //
  }
  stop();           //
  start();          //
  writeByte(Cmd_DispCtrl);//
  stop();           //
}

void TM1651::clearDisplay(void)
{
  displayLevel(0);
  frame(FRAME_OFF);
}
//To take effect the next time it displays.
void TM1651::set(uint8_t brightness,uint8_t SetData,uint8_t SetAddr)
{
  Cmd_SetData = SetData;
  Cmd_SetAddr = SetAddr;
  Cmd_DispCtrl = 0x88 + brightness;//Set the brightness and it takes effect the next time it displays.
}

