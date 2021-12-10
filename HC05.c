#include "HC05.h"
void UsartConfiguration()
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0x41;		//设置定时初始值
	TH1 = 0xFD;		//设置定时初始值
	//ET1 = 0;		//禁止定时器%d中断
	//TR1 = 1;		//定时器1开始计时
	ES = 1;  //打开接收中断
  EA = 1;  //打开总中断
  TR1 = 1; //打开计数器1
}
void PostChar(uchar character)  
{     
	    SBUF=character;   //发送单个字符
	    while(!TI);TI=0; //发送完成标志 
}
void Delay50us()		//@27.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 2;
	j = 76;
	do
	{
		while (--j);
	} while (--i);
}
void Delay100ms()		//@27.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 11;
	j = 67;
	k = 102;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
