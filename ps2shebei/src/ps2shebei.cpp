/*
ps/2协议在主机端和设备端不是完全一样的，在通讯时有差别设备。
 本库作为ps/2的设备端使用，也就是模拟一个ps/2设备。
 限制:不处理奇偶校验错误。时序常数根据规范硬编码。
 数据穿输速率并不是很快，可能还有很大的优化空间。
 */
#include "ps2shebei.h"
//下面两个定义是时钟周期的长度。
#define CLKFULL 40
//从阶段开始到驱动数据线的时间。
#define CLKHALF 20

 //arduino能正确设置时钟引脚和数据引脚的状态，因此不需要额外的上拉电阻。
PS2SHEBEI::PS2SHEBEI(int clock, int data)
{
  ps2clock = clock;
  ps2data = data;
  high(ps2clock);
  high(ps2data);
}

void PS2SHEBEI::high(int pin)
{
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void PS2SHEBEI::low(int pin)
{
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

//向ps/2主机写入一个字节。
int PS2SHEBEI::write(unsigned char data)
{
  unsigned char i;
  unsigned char parity = 1;

  if (digitalRead(ps2clock) == LOW) {
    return -1;
  }
  if (digitalRead(ps2data) == LOW) {
    return -2;
  }

  ////发送起始位。
  low(ps2data);
  delayMicroseconds(CLKHALF);
  //拉低时钟线电平，等待一段时间拉高时钟线电平。
  low(ps2clock);
  delayMicroseconds(CLKFULL);
  high(ps2clock);
  delayMicroseconds(CLKHALF);

  for (i=0; i < 8; i++)
    {
      //发送数据位。
      if (data & 0x01)
      {
        high(ps2data);
      } else
      {
        low(ps2data);
      }
      delayMicroseconds(CLKHALF);
      //拉低时钟线电平，等待一段时间拉高时钟线电平。
      low(ps2clock);	
      delayMicroseconds(CLKFULL);
      high(ps2clock);
      delayMicroseconds(CLKHALF);

      parity = parity ^ (data & 0x01);
      data = data >> 1;
    }
  //发送奇偶校验位。
  if (parity)
    {
      high(ps2data);
    } else{
      low(ps2data);
      }
  delayMicroseconds(CLKHALF);
  //拉低时钟线电平，等待一段时间拉高时钟线电平。
  low(ps2clock);	
  delayMicroseconds(CLKFULL);
  high(ps2clock);
  delayMicroseconds(CLKHALF);

  //发送停止位
  high(ps2data);
  delayMicroseconds(CLKHALF);
  //拉低时钟线电平，等待一段时间拉高时钟线电平。
  low(ps2clock);	
  delayMicroseconds(CLKFULL);
  high(ps2clock);
  delayMicroseconds(CLKHALF);

  delayMicroseconds(50);
  return 0;
}

//从ps2主机读取一个字节的数据。忽略停止位。
int PS2SHEBEI::read(unsigned char * value)
{
  unsigned char data = 0x00;
  unsigned char i;
  unsigned char bit = 0x01;
  unsigned char parity = 1;
  
  //ps/2主机发送开始位
  //等待ps/2主机拉低数据线电平 
  while (digitalRead(ps2data) == HIGH){
  } 
  //等待ps/2主机拉高时钟线电平
  while (digitalRead(ps2clock) == LOW){
  } 

  delayMicroseconds(CLKHALF);
  low(ps2clock);
  delayMicroseconds(CLKFULL);
  high(ps2clock);
  delayMicroseconds(CLKHALF);

  //ps/2主机发送数据位。
  for (i=0; i < 8; i++)
    {
      if (digitalRead(ps2data) == HIGH){
        data = data | bit;
        }
        else{
        }

      bit = bit << 1;
      
      delayMicroseconds(CLKHALF);
      //拉低时钟线电平，等待一段时间拉高时钟线电平。
      low(ps2clock);	
      delayMicroseconds(CLKFULL);
      high(ps2clock);
      delayMicroseconds(CLKHALF);
      
      //我们在一轮循环将结束时进行奇偶校验。
      parity = parity ^ (data & 0x01);
    }

  //不接收奇偶校验位
  delayMicroseconds(CLKHALF);
  low(ps2clock);	
  delayMicroseconds(CLKFULL);
  high(ps2clock);
  delayMicroseconds(CLKHALF);
  
  //抑制ps/2主机通讯，不接收停止位。
  delayMicroseconds(CLKHALF);
  low(ps2data);
  
  //拉低时钟线电平，等待一段时间拉高时钟线电平。
  low(ps2clock);	
  delayMicroseconds(CLKFULL);
  high(ps2clock);
  delayMicroseconds(CLKHALF);

  //释放数据线
  high(ps2data);

  *value = data;
  
  return 0;
}

