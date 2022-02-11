/*
 *ps/2协议设备端的库。
 *Chris J.Kiick撰写，2008年1月发布到公共领域。
 *Gene E.Scogin修饰，2008年8月发布到公共领域。
 *2022年2月由寂寞的欧尼酱做简单修改，更改注释。
 */
#ifndef ps2shebei_h
#define ps2shebei_h
#include "arduino.h"

class PS2SHEBEI
{
	public:
		PS2SHEBEI(int clock, int data);
		int write(unsigned char data);
		int read(unsigned char * data);
	private:
		int ps2clock;
		int ps2data;
		void low(int pin);
		void high(int pin);
};
#endif

