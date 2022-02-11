# arduino-ps/2库
&emsp;&emsp;适合arduino开发板使用的ps/2库，用于ps/2协议通讯，使用c语言编写。ps/2协议在主机端和设备端不是完全一样，在通讯时有差别。

仓库里有一份Synaptics PS/2 TouchPad Interfacing Guide（可惜是英文的），非常适合用来学习ps/2协议和触摸板通讯。还有其他的资料若干。
# ps2zhuji
&emsp;&emsp;本库作为ps/2的主机端使用，用做与ps/2设备通讯。 

&emsp;&emsp;我（寂寞的欧尼酱）抄了Synaptics PS/2 TouchPad Interfacing Guide上的示例代码，将示例代码和ps2库合并，并做出一些更改，让它更适合arduino的使用以及符合我的习惯。

&emsp;&emsp;根据Synaptics PS/2 TouchPad Interfacing Guide上的描述，触控板每秒发送40个或者80个数据包，代码的数据穿输速率应该足够了。

&emsp;&emsp;读取字节的奇偶校验可以选择处理还是不处理，不处理奇偶校验就把相关代码注释掉，默认是不处理。

&emsp;&emsp;读取或者写入时遇到错误时，有相关的代码报告错误，具体看附近的注释，默认是注释掉。

&emsp;&emsp;Chris J.Kiick撰写，2008年1月发布到公共领域。

&emsp;&emsp;Gene E.Scogin修饰，2008年8月发布到公共领域。

&emsp;&emsp;2022年2月由寂寞的欧尼酱做简单修改，更改注释，并发布到GitHub。
# ps2shebei
&emsp;&emsp;ps/2协议设备端的库。本库作为ps/2的设备端使用，也就是模拟一个ps/2设备。

&emsp;&emsp;限制:不处理奇偶校验错误。时序常数根据规范硬编码。数据穿输速率并不是很快，可能还有很大的优化空间。

&emsp;&emsp;Chris J.Kiick撰写，2008年1月发布到公共领域。

&emsp;&emsp;Gene E.Scogin修饰，2008年8月发布到公共领域。

&emsp;&emsp;2022年2月由寂寞的欧尼酱做简单修改让它符合我的习惯，翻译注释方便后来人。并发布到GitHub。
