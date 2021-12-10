#include "DS18B20.h"

void Delay1ms(unsigned int y)
{
	int x;
	for (x = 0; x < y; x++)
	{
		unsigned char i, j;

		i = 27;
		j = 64;
		do
		{
			while (--j)
				;
		} while (--i);
	}
}

unsigned char Ds18b20Init()
{
	unsigned int i;
	DSPORT = 0;	  //将总线拉低480us~960us
	Delay642us(); //延时642us
	DSPORT = 1;	  //然后拉高总线，如果DS18B20做出反应会将在15us~60us后总线拉低
	i = 0;
	while (DSPORT) //等待DS18B20拉低总线
	{
		Delay5ms(); //等待>5MS
		return 0;	//初始化失败
	}
	return 1; //初始化成功
}

void Ds18b20WriteByte(unsigned char dat)
{
	unsigned int i, j;
	for (j = 0; j < 8; j++)
	{
		DSPORT = 0; //每写入一位数据之前先把总线拉低1us
		i++;
		DSPORT = dat & 0x01; //然后写入一个数据，从最低位开始
		Delay68us();		 //延时68us，持续时间最少60us
		DSPORT = 1;			 //然后释放总线，至少1us给总线恢复时间才能接着写入第二个数值
		dat >>= 1;
	}
}

unsigned char Ds18b20ReadByte()
{
	unsigned char byte, bi;
	unsigned int i, j;
	for (j = 8; j > 0; j--)
	{
		DSPORT = 0; //先将总线拉低1us
		i = 4;
	    while (--i);
		DSPORT = 1; //然后释放总线
		i = 38;
	    while (--i);		 //延时6us等待数据稳定
		bi = DSPORT; //读取数据，从最低位开始读取
		/*将byte左移一位，然后与上右移7位后的bi，注意移动之后移掉那位补0。*/
		byte = (byte >> 1) | (bi << 7);
		Delay48us(); //读取完之后等待48us再接着读取下一个数
	}
	return byte;
}

void Ds18b20ChangTemp()
{
	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc); //跳过ROM操作命令
	Ds18b20WriteByte(0x44); //温度转换命令
	//	Delay1ms(100);	//等待转换成功，而如果你是一直刷着的话，就不用这个延时了
}

void Ds18b20ReadTempCom()
{

	Ds18b20Init();
	Delay1ms(1);
	Ds18b20WriteByte(0xcc); //跳过ROM操作命令
	Ds18b20WriteByte(0xbe); //发送读取温度命令
}

short Ds18b20ReadTemp()
{
	unsigned char temp = 0;
	unsigned char tmh, tml;
	short tem;

	Ds18b20ChangTemp();		 //先写入转换命令
	Ds18b20ReadTempCom();	 //然后等待转换完后发送读取温度命令
	tml = Ds18b20ReadByte(); //读取温度值共16位，先读低字节
	tmh = Ds18b20ReadByte(); //再读高字节

	if (tmh > 7)
	{
		tmh = ~tmh;
		tml = ~tml;
		temp = 0; //温度为负
	}
	else
	{
		temp = 1; //温度为正
	}

	tem = tmh; //获得高八位
	tem <<= 8;
	tem |= tml;				   //获得底八位
	tem = (double)tem * 0.625; //转换   放大10倍  精度0.1
	if (temp)
		return tem; //返回温度值
	else
		return -tem;
}
void Delay642us() //@27.000MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 17;
	j = 217;
	do
	{
		while (--j)
			;
	} while (--i);
}
void Delay5ms() //@27.000MHz
{
	unsigned char i, j;

	i = 132;
	j = 79;
	do
	{
		while (--j)
			;
	} while (--i);
}
void Delay68us() //@27.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 198;
	do
	{
		while (--j)
			;
	} while (--i);
}
void Delay48us() //@27.000MHz
{
	unsigned char i, j;

	i = 2;
	j = 63;
	do
	{
		while (--j)
			;
	} while (--i);
}
