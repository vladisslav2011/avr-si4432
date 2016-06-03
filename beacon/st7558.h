/*



#define st7558_resetPORT A //optional
#define st7558_resetBIT 0  //optional
#define st7558_sa0PORT A
#define st7558_sa0BIT 8

#define i2cpfix st7558
#define i2c_delay() _delay_us(400)

#define i2c_PORT A
#define i2c_SDA_BIT 0
#define i2c_SCL_BIT 1

#include "softI2C.h"
#undef i2cpfix
#undef i2c_delay
#undef i2c_PORT
#undef i2c_SDA_BIT
#undef i2c_SCL_BIT

or implement 

st7558_i2cINIT()
st7558_i2cSTART()
st7558_i2cSTOP()
unsigned char st7558_i2cRX(unsigned char ack)
unsigned char st7558_i2cTX(unsigned char v)

with any of hardware i2c interfaces


*/
#define __concat(a,b) a##b
#define _concat(a,b) __concat(a,b)

#define st7558_address 0x78


void st7558_reset()
{
#ifdef st7558_resetPORT
	_concat(PORT,st7558_resetPORT) &= ~(1<<st7558_resetBIT);
	_concat(DDR,st7558_resetPORT) |= (1<<st7558_resetBIT);
	_concat(PORT,st7558_sa0PORT) &= ~(1<<st7558_sa0BIT);
	_concat(DDR,st7558_sa0PORT) |= (1<<st7558_sa0BIT);
	_delay_us(2);
	_concat(PORT,st7558_resetPORT) |= (1<<st7558_resetBIT);
	_delay_us(2);
#endif
}

#define st7558_start() st7558_i2cSTART()


#define st7558_write(c)	st7558_i2cTX(c)


#define  st7558_read() st7558_i2cRX(1)

#define st7558_mode(a)\
do{\
	st7558_start();\
	st7558_write(0x78);\
	st7558_write(0x00);\
	if(a)\
		st7558_write(0x08 |0x05);\
	else\
		st7558_write(0x08 |0x04);\
	st7558_i2cSTOP();\
}while(0)

#define st7558_clear()\
	do{\
		int i;\
		st7558_start();\
		st7558_write(0x78);\
		st7558_write(0x40);\
		for(i = 0; i < 9*101; i++)\
			st7558_write(0x00);\
		st7558_i2cSTOP();\
	}while(0)


char st7558_init()
{
	st7558_i2cINIT();
	st7558_reset();
	st7558_start();
	if(!st7558_write(0x78))//address+write
	{
		st7558_i2cSTOP();
		return 1;
	};
	st7558_write(0x00);
	st7558_write(0x2e);//mirror x+y
	st7558_write(0x21);//horizontal addressing,exit power down,ext instr set
	st7558_write(0x0e);// s/w initial internal register 1st instr???
	st7558_write(0x12);// s/w initial internal register 2nd instr???
	st7558_write(0x08|0x00); //set booster voltage
	_delay_ms(50);
	st7558_write(0x80|0x20); //set Vop to 0x20
	st7558_write(0x10|0x04); //set BIAS to 7 (1/40:1/34)
	//st7558_write(0x08|0x03); //set booster voltage to 5*3.3=16,5???
	st7558_write(0x08|0x01); //set booster voltage
	st7558_write(0x20);//normal instr set
	st7558_write(0x10|0x01);//set vlcd programming range to high
	st7558_write(0x08 |0x05);//set inverse video mode
	st7558_write(0x40 | 0x00);//set y pos to 0
	st7558_write(0x80 | 0x00);//set x pos to 0
	st7558_i2cSTOP();
	
	st7558_start();
	st7558_write(0x78);
	st7558_write(0x00);
	st7558_write(0x20);//normal instr set
	st7558_write(0x08| 0x00); //display off
	st7558_i2cSTOP();
		
	st7558_mode(0);
	st7558_clear();
	return 0;
}

void st7558_setpos(unsigned char x,unsigned char y)
{
	st7558_start();
	st7558_write(0x78);
	st7558_write(0x00);
	st7558_write(0x40 | (y&0x3f));//set y
	st7558_write(0x80 | (x&0x7f));//set x
	st7558_i2cSTOP();
}

void st7558_putbyte(unsigned char b)
{
	st7558_start();
	st7558_write(0x78);
	st7558_write(0x40);
	st7558_write(b);
	st7558_i2cSTOP();
}

void st7558_putstr(unsigned char * b, unsigned char len)
{
	st7558_start();
	st7558_write(0x78);
	st7558_write(0x40);
	unsigned char k;
	for(k=0;k<len;k++)
		st7558_write(b[k]);
	st7558_i2cSTOP();
}




#undef _concat
#undef __concat

