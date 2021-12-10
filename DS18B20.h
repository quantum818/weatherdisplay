#include "STC15FX.h"
#include "intrins.h"
#define DSPORT P34
void Delay1ms(unsigned int );
unsigned char Ds18b20Init();
void Ds18b20WriteByte(unsigned char com);
unsigned char Ds18b20ReadByte();
void  Ds18b20ChangTemp();
void  Ds18b20ReadTempCom();
short Ds18b20ReadTemp();
void Delay642us(); //@27.000MHz
void Delay5ms(); //@27.000MHz
void Delay68us(); //@27.000MHz
void Delay48us(); //@27.000MHz