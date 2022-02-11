#include <ps2.h>
/*
 * 作者：寂寞的欧尼酱
 * 日期：2022年2月10日
 *这个示例适用于ps/2协议的鼠标及触控板
 *触控板连接arduino的针脚，可根据自己的需要更改。
 *ps/2设备电源可以接在开发板上，也可以接其他地方。
 */
#define DATA 5
#define CLOCK 6

//建立触控板对象。
PS2 mouse(CLOCK, DATA);

void setup()
{
  //设备初始化，可以调整触摸板相关参数。
  //初始化开始
  //进入Reset模式
  mouse.write(0xff);
  
  //读取自检情况
  mouse.read();

  //读取触控板ID
  mouse.read();
  
  //进入Remote模式
  mouse.write(0xf0);
  
  //初始化结束。
  //开启串口发送数据
  Serial.begin(9600);
}

void loop() {
  //设备会发送一个三字节的数据包，用三个变量保存。
  int m;
  int x;
  int y;

  //获取触摸板数据
  mouse.write(0xeb);
  //保存获取的数据
  m = mouse.read();
  x = mouse.read();
  y = mouse.read();

  Serial.print(m, BIN);
  Serial.print("\n");
  
  Serial.print(x, BIN);
  Serial.print("\n");
  
  Serial.print(y, BIN);
  Serial.print("\n");

}
