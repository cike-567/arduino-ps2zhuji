/* 
 *我（寂寞的欧尼酱）抄了Synaptics PS/2 TouchPad Interfacing Guide上的示例代码，
 *将示例代码和原本的ps2库合并，并做出一些更改，让它更适合arduino的使用以及符合我的习惯。
 *ps/2协议在主机端和设备端不是完全一样，在通讯时有差别。
 *本库作为ps/2的主机端使用，用做与ps/2设备通讯。 
 *根据Synaptics PS/2 TouchPad Interfacing Guide上的描述，触控板每秒发送40个或者80个数据包，代码的数据穿输速率应该足够了。
 *读取字节的奇偶校验可以选择处理还是不处理，不处理奇偶校验就把相关代码注释掉，默认是不处理。
 *读取或者写入时遇到错误时，有相关的代码报告错误，具体看附近的注释，默认是注释掉。
 */
#include "ps2.h"
//arduino能正确设置时钟引脚和数据引脚的状态，因此不需要额外的上拉电阻或其他部件。
//PS/2设备的电源引脚和接地引脚连接到开发板的相应位置，或者其他电源。
PS2::PS2(int clock, int data)
{
	ps2clock = clock;
	ps2data = data;
	high(ps2clock);
	high(ps2data);
}
//设置引脚高电平
void PS2::high(int pin)
{
	pinMode(pin, INPUT);
	digitalWrite(pin, HIGH);
}

//设置引脚低电平
void PS2::low(int pin)
{
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
}

//向PS/2设备写入一个字节
void PS2::write(unsigned char data)
{
	unsigned char i;
	unsigned char parity = 1;
	unsigned char ack;
	
	//抑制ps/2设备通讯
	low(ps2clock);
	delayMicroseconds(100);
	//抑制ps/2设备通讯
	low(ps2data);
	//发送起始位。
	high(ps2clock);	
	//发送数据。
	for (i=0; i < 8; i++)
	{
		//等待ps/2设备时钟脉冲。
		while (digitalRead(ps2clock) != LOW);
		//输出第i个数据位。
		if (data & 0x01)
		{
			high(ps2data);
		} else {
			low(ps2data);
		}
		parity = parity + data;
		//等待ps/2设备时钟脉冲结束。
		while (digitalRead(ps2clock) != HIGH);
		data = data >> 1;
	}
	while (digitalRead(ps2clock) != LOW);
	//发送奇偶校验位。
	if (parity & 0x01)
	{
		high(ps2data);
	} else {
		low(ps2data);
	}
	//等待奇偶检验位时钟结束。
	while (digitalRead(ps2clock) != HIGH);
	//停止位时钟开始
	while (digitalRead(ps2clock) != LOW);
	//发送停止位（停止位必须为1）。
	high(ps2data);
	//等待停止位时钟结束。
	while (digitalRead(ps2clock) != HIGH);
	//线路控制时钟开始。
	while (digitalRead(ps2clock) != LOW);
	//这个是报告线控制位出错的代码，默认是注释掉这部分代码。
	/*
	if (digitalRead(ps2clock) == 1){
		//报告的方法不限，可以是点亮一颗LED，也可以是串口发送字符。
		//不建议使用串口发送字符的方式，占内存比较多。
		serial.print("写入字节时缺少线控制位！\n");
	}
	*/
	//等待线路控制时钟结束。
	while (digitalRead(ps2clock) != HIGH);
	//检查ps/2设备是否接收字节，默认是注释掉这部分代码。
	//ps/2触摸板接收命令时会返回一个FA，接收字节出错时，会返回其他字节。
	//下面这一行代码负责接收返回的字节，这样就不需要在项目手动接收字节。
	ack = read();
	//校验接收到的字节，如果不是FA，就报告给主机。可以根据接收的字节判断问题所在。
	//报告的方法不限，可以是点亮一颗LED，也可以是串口发送字符。
	//不建议使用串口发送字符的方式，占内存比较多。
	//默认是注释掉的。
	/*
	if (ack != 0xFA){
		serial.print(ack,HEC);
	}
	*/
}
//从ps2设备读取一个字节。
unsigned char PS2::read(void)
{
	unsigned char i;
	unsigned char bit;
	unsigned char data = 0x00;
	//用作奇偶检验的变量，默认注释掉
	//unsigned char parity = 0;
	//ps/2通讯抑制解除。
	high(ps2clock);
	//等待ps/2设备发送起始位。
	while (digitalRead(ps2clock) != LOW);
	//起始位发送结束。
	while (digitalRead(ps2clock) != HIGH);	
	for (i=0; i < 8; i++)
	{
		//等待ps/2设备的时钟脉冲。
		while (digitalRead(ps2clock) != LOW);
		bit = digitalRead(ps2data);
		data = data + (bit << i);
		//将奇偶校验位累加为奇偶校验位。默认注释掉。
		//parity = parity + bit;
		//等待ps/2设备的时钟脉冲。
		while (digitalRead(ps2clock) != HIGH);
	}
	//奇偶校验位时钟开始。
	while (digitalRead(ps2clock) != LOW);
	//将奇偶校验位累加为奇偶校验位。默认注释掉。
	//parity = parity + digitalRead(ps2data);
	//这个是报告奇偶检验位出错的代码，默认是注释掉这部分代码。
	/*
	if ((parity & 0x01) == 0){
		//报告的方法不限，可以是点亮一颗LED，也可以是串口发送字符。
		//不建议使用串口发送字符的方式，占内存比较多。
		serial.print("读取字节时，奇偶校验错误！\n");
	}
	*/
	//奇偶校验位时钟结束
	while (digitalRead(ps2clock) != HIGH);
	//停止位时钟。
	while (digitalRead(ps2clock) != LOW);
	//这个是报告停止位出错的代码，默认是注释掉这部分代码。
	/*
	if (read_DATA() == 0){
	    //报告的方法不限，可以是点亮一颗LED，也可以是串口发送字符。
		//不建议使用串口发送字符的方式，占内存比较多。
		serial.print("读取数据时，停止位错误！\n");
	}
	*/
	//ps/2通讯抑制。
	low(ps2clock);
	//等待最后的时钟脉冲。
	delayMicroseconds(50);
	//返回接收到的数据。
	return data;
}

