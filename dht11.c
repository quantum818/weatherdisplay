#include "dht11.h"
void Delay1000ms()//定义延时为1s的延时函数		
{

	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 103;
	j = 153;
	k = 44;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);


}

void Start()
{
	DATA=0;//将总线拉低
	Delay20ms();//延时20ms，确保延时大于18ms，这样DHT11可以检测到起始信号，然后就可以开始发出响应信号
	DATA=1;//将总线拉高
	Delay30us();
	DATA=1;
}

int readdht11_byte() //定义一个以字节为单位的函数，分别读取dht11发送的40bits的数据
{   
  int i,dht;  
  for(i=8;i>0;i--)//下面的这个循环实现读取八个bit的数据
  {      
    dht=dht<<1;//每次读取前，将数据的高位移位向左移位
    while(!DATA);//等待DHT11的50us低电平数据分隔信号过去
	Delay35us();//数据低电平分隔信号过去后，等待35us，然后再判断，具体为什么要等待，待下文分晓
    if(DATA==1) //如果延时35us后，信号还是高电平的话，那么就执行if里面的程序
  	{
      dht=dht|0x01;//dht的最低位置1，其他的位不改变
      Delay55us();//再延时55us，等待高电平信号过去
    }   
    else;//如果延时35us后，是低电平的话，那么就什么都不执行，在下一次if循化，dht数据移位的时候，会自动把最低位置为0，然后向左移位             
  } 
  return(dht);//将这个dht返回   
}

void read_dht11() 
{
  Start();
  if(!DATA)
  {   
    while(!DATA);//等待低电平响应信号过去
    while(DATA);//再等待DHT11低电平响应信号之后的一段80us的高电平确认信号过去
    RHH=readdht11_byte();//读取湿度的整数位
    RLL=readdht11_byte();//读取湿度的小数位
    CHH=readdht11_byte();//读取温度的整数位
    CLL=readdht11_byte();//读取温度的小数位
   }  
}
void Trans()//将读取的温湿度转换成字符型
{
	sprintf(TEMPH,"%d",CHH);//将温度整数位转换成字符
	sprintf(TEMPL,"%d",CLL/1000);//将温度的小数位转换成字符
	sprintf(DAMPH,"%d",RHH);
	sprintf(DAMPL,"%d",RLL/10000);
	strcat(TEMPH,dot);//将小数点和温度的整数位合成一串字符
	strcat(TEMPH,TEMPL);//将合成后的温度整数位+小数点与温度的小数位合成一串字符
	strcat(DAMPH,dot);
	strcat(DAMPH,DAMPL);
}
void Delay20ms()		//@27.000MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 3;
	j = 14;
	k = 67;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}
void Delay30us()		//@27.000MHz
{
	unsigned char i;

	i = 200;
	while (--i);
}
void Delay35us()		//@27.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	_nop_();
	i = 233;
	while (--i);
}
void Delay55us()		//@27.000MHz
{
	unsigned char i, j;

	_nop_();
	i = 2;
	j = 110;
	do
	{
		while (--j);
	} while (--i);
}

