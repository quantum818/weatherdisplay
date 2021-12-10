#include "OLED12864.h"
void delay(unsigned int z)
{
	unsigned int x, y;
	for (x = z; x > 0; x--)
		for (y = 110; y > 0; y--)
			;
}

/**********************************************
//IIC Start
**********************************************/
void IIC_Start()
{
	SCL = high;
	SDA = high;
	SDA = low;
	SCL = low;
}

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop()
{
	SCL = low;
	SDA = low;
	SCL = high;
	SDA = high;
}

/**********************************************
// 通过IIC发送数据
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		if (IIC_Byte & 0x80)
			SDA = high;
		else
			SDA = low;
		SCL = high;
		SCL = low;
		IIC_Byte <<= 1;
	}
	SDA = 1;
	SCL = 1;
	SCL = 0;
}

/*向OLED发送数据*/
void OLED_WrDat(unsigned char IIC_Data)
{
	IIC_Start();
	Write_IIC_Byte(0x78);
	Write_IIC_Byte(0x40); //write data
	Write_IIC_Byte(IIC_Data);
	IIC_Stop();
}
/*向OLED发送地址指令*/
void OLED_WrCmd(unsigned char IIC_Command)
{
	IIC_Start();
	Write_IIC_Byte(0x78); //Slave address,SA0=0
	Write_IIC_Byte(0x00); //write command
	Write_IIC_Byte(IIC_Command);
	IIC_Stop();
}
/*设定显示初始位置*/
void OLED_Set_Pos(unsigned char x, unsigned char y)
{
	OLED_WrCmd(0xb0 + y);
	OLED_WrCmd(((x & 0xf0) >> 4) | 0x10);
	OLED_WrCmd((x & 0x0f) | 0x01);
}

/*屏幕清空*/
void OLED_CLS(void)
{
	unsigned char y, x;
	for (y = 0; y < 8; y++)
	{
		OLED_WrCmd(0xb0 + y);
		OLED_WrCmd(0x01);
		OLED_WrCmd(0x10);
		for (x = 0; x < X_WIDTH; x++)
			OLED_WrDat(0);
	}
}
/*屏幕初始化*/
void OLED_Init(void)
{
	delay(500);				//???????????!
	OLED_WrCmd(0xae);		//--turn off oled panel
	OLED_WrCmd(0x00);		//---set low column address
	OLED_WrCmd(0x10);		//---set high column address
	OLED_WrCmd(0x40);		//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	OLED_WrCmd(0x81);		//--set contrast control register
	OLED_WrCmd(Brightness); // Set SEG Output Current Brightness
	OLED_WrCmd(0xa1);		//--Set SEG/Column Mapping     0xa0???? 0xa1??
	OLED_WrCmd(0xc8);		//Set COM/Row Scan Direction   0xc0???? 0xc8??
	OLED_WrCmd(0xa6);		//--set normal display
	OLED_WrCmd(0xa8);		//--set multiplex ratio(1 to 64)
	OLED_WrCmd(0x3f);		//--1/64 duty
	OLED_WrCmd(0xd3);		//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	OLED_WrCmd(0x00);		//-not offset
	OLED_WrCmd(0xd5);		//--set display clock divide ratio/oscillator frequency
	OLED_WrCmd(0x80);		//--set divide ratio, Set Clock as 100 Frames/Sec
	OLED_WrCmd(0xd9);		//--set pre-charge period
	OLED_WrCmd(0xf1);		//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	OLED_WrCmd(0xda);		//--set com pins hardware configuration
	OLED_WrCmd(0x12);
	OLED_WrCmd(0xdb); //--set vcomh
	OLED_WrCmd(0x40); //Set VCOM Deselect Level
	OLED_WrCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
	OLED_WrCmd(0x02); //
	OLED_WrCmd(0x8d); //--set Charge Pump enable/disable
	OLED_WrCmd(0x14); //--set(0x10) disable
	OLED_WrCmd(0xa4); // Disable Entire Display On (0xa4/0xa5)
	OLED_WrCmd(0xa6); // Disable Inverse Display On (0xa6/a7)
	OLED_WrCmd(0xaf); //--turn on oled panel
	OLED_Set_Pos(0, 0);
}

/*显示ACSII字符串*/
void OLED_P8x16Str(unsigned char x, unsigned char y, unsigned char ch[])
{
	unsigned char c = 0, i = 0, j = 0;
	while (ch[j] != '\0')
	{
		c = ch[j] - 32;
		if (x > 120)
		{
			x = 0;
			y++;
		}
		OLED_Set_Pos(x, y);
		for (i = 0; i < 8; i++)
			OLED_WrDat(F8X16[c * 16 + i]);
		OLED_Set_Pos(x, y + 1);
		for (i = 0; i < 8; i++)
			OLED_WrDat(F8X16[c * 16 + i + 8]);
		x += 8;
		j++;
	}
}
//18x20
//显示单个汉字字符
void OLED_CN(unsigned char x, unsigned char y, unsigned char *ch)
{
	int getla = 0;
	unsigned char c = 0, i = 0, j = 0, t, adder = 0;
	for (i = 0; i < 1; i++)
	{
		if ((CN16CHAR[i].Index[0] == *ch) && (CN16CHAR[i].Index[1] == *(ch + 1)))
		{
			getla = i;
		}
	}
	OLED_Set_Pos(x, y);
	for (t = 0; t < 16; t++)
		OLED_WrDat(CN16CHAR[getla].Msk[t]); //写入字模

	OLED_Set_Pos(x, y + 1);
	for (t = 16; t < 32; t++)
		OLED_WrDat(CN16CHAR[getla].Msk[t]);
}
//显示汉字字符串
void OLED_CNStr(unsigned char x, unsigned char y, unsigned char *ch)
{
	int getla = 0;
	int c = 0, i = 0, j = 0, t, adder = 0;
	while (*ch != '\0')
	{
		getla=0;
		for (i = 0; i < 29; i++)
		{
			if ((CN16CHAR[i].Index[0] == *ch) && (CN16CHAR[i].Index[1] == *(ch + 1))&&(CN16CHAR[i].Index[2] == *(ch + 2)) )
			{
				getla = i;
				break;
			}
		}
		OLED_Set_Pos(x, y);
		for (t = 0; t < 16; t++)
			OLED_WrDat(CN16CHAR[getla].Msk[t]); //写入字模

		OLED_Set_Pos(x, y + 1);
		for (t = 16; t < 32; t++)
			OLED_WrDat(CN16CHAR[getla].Msk[t]);
		x = x + 16;
		ch+=3;
	}
}
//显示128*64二值图片
void Draw_BMP(unsigned char x0, unsigned char y0, unsigned char x1, unsigned char y1, unsigned char BMP[])
{
	unsigned int j = 0;
	unsigned char x, y;

	if (y1 % 8 == 0)
		y = y1 / 8;
	else
		y = y1 / 8 + 1;
	for (y = y0; y < y1; y++)
	{
		OLED_Set_Pos(x0, y);
		for (x = x0; x < x1; x++)
		{
			OLED_WrDat(BMP[j++]);
		}
	}
}