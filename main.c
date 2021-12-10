#include "STC15FX.h"
#include "OLED12864.h"
#include "HC05.h"
#include "IMG.h"
#include "dht11.h"
#include "DS18B20.h"
#define uchar unsigned char
#define S1_S0 0x40 //P_SW1.6
#define S1_S1 0x80 //P_SW1.7
uchar receiveData; //接收到的数据
int getinfo[7] = {0};
int times = 0;
int statser = 1;
int ifget = 1, ifreturn = 1;
int ifgetcompete = 0; //数据是否全部传输完了
int temp = 0;
int butten1 = 0; //判断按钮1是否按下
int butten2 = 0; //判断按钮2是否按下
int RLL, RHH, CLL, CHH;
unsigned char weatherinfo[10]; //天气状态
unsigned char otherinfo[6];    //数字量
unsigned char competeinfo[6];
char DAMPH[8], TEMPH[8], DAMPL[8], TEMPL[8];
char dot[1] = ".";
void dealweatherinfo(unsigned char ind);
void dealnumners(int ind);
short tem;
void main();
void main()
{
    ACC = P_SW1;             //外部硬件引腳選擇
    ACC &= ~(S1_S0 | S1_S1); //S1_S0=1 S1_S1=0
    ACC |= S1_S0;            //(P3.6/RxD_2, P3.7/TxD_2)
    P_SW1 = ACC;
    UsartConfiguration();
    OLED_Init(); //初始化OLED
    OLED_CLS();  //OLED清屏
    TI = 0;
    while (1)
    {
        int Q = 100, p = 0, temps = 20, localdisplay = 0, displayagain = 0;
        Delay50us();
        if (P33 == 0)
        {
            butten1 = 1;
            ifreturn = 1;
        }
        Delay50us();
        if (P32 == 0)
        {
            butten2 = 1;
            localdisplay = 1;
            displayagain = 1;
            butten1 = 0;
        }
        if (statser && butten1)
        {
            if (ifreturn)
            {
                OLED_CLS(); //OLED清屏
            }
            while (temps--)
            { //向上位机发送50次0x43
                PostChar(67);
                Delay50us(); //延时防止数据错乱
            }
            Draw_BMP(0, 0, 128, 8, gImage_tt);
            ifreturn = 0;
        }
        //输出获得的天气代码
        //1.天气情况2.体感温度3.风速4.气压5.云量
        else if (butten1)
        {
            if (ifget)
            {
                OLED_Init(); //初始化OLED
                OLED_CLS();  //OLED清屏
            }
            if (ifgetcompete)
            {
                ifget = 0;
                dealweatherinfo(getinfo[0]);
                if (getinfo[0] != 0)
                {
                    OLED_CNStr(0, 0, "天气\0");
                    OLED_CNStr(35, 0, weatherinfo);
                }
                dealnumners(getinfo[1]);
                OLED_CNStr(0, 2, "体感温度\0");
                OLED_P8x16Str(67, 2, competeinfo);
                dealnumners(getinfo[4] * 10 + getinfo[5]);
                OLED_CNStr(0, 4, "气压\0");
                OLED_P8x16Str(35, 4, competeinfo);
                dealnumners(getinfo[3]);
                OLED_CNStr(0, 6, "云量\0");
                OLED_P8x16Str(35, 6, competeinfo);
                dealnumners(getinfo[2]);
                OLED_CNStr(56, 6, "风速\0");
                OLED_P8x16Str(91, 6, competeinfo);
                ifgetcompete = 0;
            }
        }
        //按钮2按下显示传感器温湿度
        else if (butten2)
        {
            if (localdisplay)
            {
                OLED_CLS();
                localdisplay = 0;
            }
            if (displayagain)
            {
                RHH = 0;
                RLL = 0;
                CLL = 0;
                CHH = 0;
                Delay50us();
                read_dht11();
                Delay1000ms();
                Trans();
                //tem=Ds18b20ReadTemp();
                //dealnumners(tem);
                OLED_CNStr(0, 0, "温度\0");
                OLED_P8x16Str(35, 0, TEMPH);
                OLED_CNStr(0, 4, "湿度\0");
                OLED_P8x16Str(35, 4, DAMPH);
            }
        }
        else
        {
            Draw_BMP(0, 0, 128, 8, gImage_sleep); //显示休眠图片
        }
    }
}
void Usart() interrupt 4 //串口1接收中断
{
    if (RI)
    {
        statser = 0;
        receiveData = SBUF; //出去接收到的数据
        RI = 0;             //清除接收中断标志位
        SBUF = receiveData; //将接收到的数据放入到发送寄存器
        if (receiveData != 'X')
        {
            getinfo[times] = receiveData;
            times++;
        }
        else
        {
            ifgetcompete = 1;
            ifget = 1;
        }
        while (!TI)
            ;   //等待发送数据完成
        TI = 0; //清除发送完成标志位
    }
}
//首位天气码处理
void dealweatherinfo(unsigned char ind)
{
    switch (ind)
    {
    case 'A':
        strcpy(weatherinfo, "晴\0");
        break;
    case 'B':
        strcpy(weatherinfo, "多云\0");
        break;
    case 'C':
        strcpy(weatherinfo, "少云\0");
        break;
    case 'D':
        strcpy(weatherinfo, "晴间多云\0");
        break;
    case 'E':
        strcpy(weatherinfo, "阴\0");
        break;
    case 'F':
        strcpy(weatherinfo, "阵雨\0");
        break;
    case 'G':
        strcpy(weatherinfo, "小雨\0");
        break;
    case 'H':
        strcpy(weatherinfo, "中雨\0");
        break;
    case 'I':
        strcpy(weatherinfo, "大雨\0");
        break;
    case 'J':
        strcpy(weatherinfo, "暴雨\0");
        break;
    case 'K':
        strcpy(weatherinfo, "雪\0");
        break;
    case 'L':
        strcpy(weatherinfo, "雾\0");
        break;
    case 'M':
        strcpy(weatherinfo, "霾\0");
        break;
    }
}
//将数字转换为字符数组
void dealnumners(int ind)
{
    int nowtemp;
    int i;
    int j = 0;
    temp = 0;
    strcmp(competeinfo, "");
    strcmp(otherinfo, "");
    if (ind == 0)
    {
        competeinfo[0] = '0';
        j++;
    }
    if (ind != 0)
    {
        while (ind != 0)
        {
            nowtemp = ind % 10;
            ind = ind / 10;
            otherinfo[temp] = (unsigned char)(nowtemp + 48);
            temp++;
        }
        for (i = temp - 1; i >= 0; i--)
        {
            competeinfo[j] = otherinfo[i];
            j++;
        }
    }
    competeinfo[j] = '\0';
}