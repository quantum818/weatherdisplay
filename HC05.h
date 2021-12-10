#include "STC15FX.h"
#include <string.h>
#include <stdio.h>
#include <intrins.h>
#define uchar unsigned char
#define FOSC 27000000L //System frequency
#define BAUD 9600      //UART baudrate
/*Define UART parity mode*/
#define NONE_PARITY 0  //None parity
#define ODD_PARITY 1   //Odd parity
#define EVEN_PARITY 2  //Even parity
#define MARK_PARITY 3  //Mark parity
#define SPACE_PARITY 4 //Space parity

#define PARITYBIT NONE_PARITY //Testing even parity
extern uchar receiveData;
extern uchar RECBIT;
extern int getinfo[7];
extern int times;
extern int statser;
extern void UsartConfiguration();
extern void Delay50us();
extern void Delay100ms();
extern void PostChar(uchar character);


