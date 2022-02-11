/*ps/2协议主机端的库。
 *Chris J.Kiick撰写，2008年1月发布到公共领域。
 *Gene E.Scogin修饰，2008年8月发布到公共领域。
 *2022年2月10日由寂寞的欧尼酱做简单修改，增删改注释（有一部分是注释原文翻译来的），并发布到GitHub。
 */
#ifndef ps2_h
#define ps2_h
#include "arduino.h"

class PS2
{
	public:
		PS2(int clock, int data);
		void write(unsigned char data);
		unsigned char read(void);
	private:
		int ps2clock;
		int ps2data;
		void low(int pin);
		void high(int pin);
};
#endif

