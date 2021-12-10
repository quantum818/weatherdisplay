#include "STC15FX.h"
#include "codetable.h"
sbit SCL=P2^3; //????
sbit SDA=P2^2; //????
#define Page7 0x01
#define Page6 0x02
#define Page5 0x04
#define Page4 0x08
#define Page3 0x10
#define Page2 0x20
#define Page1 0x40
#define Page0 0x80
#define high 1
#define low 0
#define	Brightness	0xCF 
#define X_WIDTH 	128
#define Y_WIDTH 	64
/*********************OLED??????????************************************/
void delay(unsigned int z);

/**********************************************
//IIC Start
**********************************************/
void IIC_Start();

/**********************************************
//IIC Stop
**********************************************/
void IIC_Stop();

/**********************************************
// 通过IIC发送数据
**********************************************/
void Write_IIC_Byte(unsigned char IIC_Byte);

/*向OLED发送数据*/
void OLED_WrDat(unsigned char IIC_Data);
/*向OLED发送地址指令*/
void OLED_WrCmd(unsigned char IIC_Command);
/*设定显示初始位置*/
void OLED_Set_Pos(unsigned char x, unsigned char y) ;
/*屏幕清空*/
void OLED_CLS(void);
/*屏幕初始化*/
void OLED_Init(void);
/*显示ACSII字符串*/
void OLED_P8x16Str(unsigned char x, unsigned char y,unsigned char ch[]);
void Draw_BMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_CN(unsigned char x, unsigned char y,unsigned char *ch);
void OLED_CNStr(unsigned char x, unsigned char y,unsigned char *ch);