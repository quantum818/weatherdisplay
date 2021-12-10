#include "STC15FX.h"
#include "intrins.h"
#include "stdio.h"
#include "string.h"
#define uint unsigned int
#define uchar unsigned char
#define DATA P44
extern int RLL,RHH,CLL,CHH; 
extern char DAMPH[8],TEMPH[8],DAMPL[8],TEMPL[8];
extern char dot[1];//小数点
void delayus(uchar n);  //定义延时为us级的延时函数  
void delay_ms(uint z); //定义延时为ms级的延时函数
void Delay1000ms();//定义延时为1s的延时函数	
void Start();//DHT11接发数据
int readdht11_byte();
void read_dht11();
void Trans();
void Delay20ms();
void Delay30us();
void Delay35us();
void Delay55us();