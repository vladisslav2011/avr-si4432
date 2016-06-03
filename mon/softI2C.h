/*

#define i2cpfix myi2c
#define i2c_delay() _delay_us(400)

#define i2c_PORT A
#define i2c_SDA_BIT 0
#define i2c_SCL_BIT 1

or

#define i2c_SDA_PORT A
#define i2c_SDA_BIT 0

#define i2c_SCL_PORT B
#define i2c_SCL_BIT 1


*/

#define __concat(a,b) a##b

#define _concat(a,b) __concat(a,b)

void _concat(i2cpfix,_i2cINIT) ()
{
#ifdef 	i2c_PORT
	_concat(DDR,i2c_PORT)&=(~(1<<i2c_SDA_BIT))&(~(1<<i2c_SCL_BIT));
	_concat(PORT,i2c_PORT)&=(~(1<<i2c_SDA_BIT))&(~(1<<i2c_SCL_BIT));
#else
	_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
	_concat(PORT,i2c_SDA_PORT)|=(1<<i2c_SDA_BIT);
	_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
	_concat(PORT,i2c_SCL_PORT)|=(1<<i2c_SCL_BIT);
#endif
}

void _concat(i2cpfix,_i2cSTART) ()
{
#ifdef 	i2c_PORT
	_concat(DDR,i2c_PORT)&=(~(1<<i2c_SDA_BIT))&(~(1<<i2c_SCL_BIT));
	i2c_delay();
	_concat(DDR,i2c_PORT)|=(1<<i2c_SDA_BIT);
	i2c_delay();
	_concat(DDR,i2c_PORT)|=(1<<i2c_SCL_BIT);
	i2c_delay();
#else
	_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
	_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
	i2c_delay();
	_concat(DDR,i2c_SDA_PORT)|=(1<<i2c_SDA_BIT);
	i2c_delay();
	_concat(DDR,i2c_SCL_PORT)|=(1<<i2c_SCL_BIT);
	i2c_delay();
#endif
}


void _concat(i2cpfix,_i2cSTOP) ()
{
#ifdef 	i2c_PORT
	_concat(DDR,i2c_PORT)|=(1<<i2c_SDA_BIT);
	_concat(DDR,i2c_PORT)&=~(1<<i2c_SCL_BIT);
	i2c_delay();
	_concat(DDR,i2c_PORT)&=~(1<<i2c_SDA_BIT);
	i2c_delay();
#else
	_concat(DDR,i2c_SDA_PORT)|=(1<<i2c_SDA_BIT);
	_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
	i2c_delay();
	_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
	i2c_delay();
#endif
}


#ifndef i2c_SDA_PORT
#define i2c_SDA_PORT i2c_PORT
#endif

#ifndef i2c_SCL_PORT
#define i2c_SCL_PORT i2c_PORT
#endif


unsigned char _concat(i2cpfix,_i2cRX) (unsigned char ack)
{

	unsigned char r=0,m=0x80;
	while(m>0)
	{
		_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
		i2c_delay();
		_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
		if(_concat(PIN,i2c_SDA_PORT)&(1<<i2c_SDA_BIT))
			r|=m;
		m>>=1;
		i2c_delay();
		_concat(DDR,i2c_SCL_PORT)|=(1<<i2c_SCL_BIT);
	}
	if(ack)
		_concat(DDR,i2c_SDA_PORT)|=(1<<i2c_SDA_BIT);
	else
		_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
	i2c_delay();
	_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
	i2c_delay();
	_concat(DDR,i2c_SCL_PORT)|=(1<<i2c_SCL_BIT);
	return r;
}

unsigned char _concat(i2cpfix,_i2cTX)(unsigned char v)
{
	unsigned char m=0x80,ret=0;
	while(m>0)
	{
		if(v&m)
			_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
		else
			_concat(DDR,i2c_SDA_PORT)|=(1<<i2c_SDA_BIT);
		m>>=1;
		i2c_delay();
		_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
		i2c_delay();
		_concat(DDR,i2c_SCL_PORT)|=(1<<i2c_SCL_BIT);
	}
	_concat(DDR,i2c_SDA_PORT)&=~(1<<i2c_SDA_BIT);
	i2c_delay();
	_concat(DDR,i2c_SCL_PORT)&=~(1<<i2c_SCL_BIT);
	i2c_delay();
	ret=( _concat(PIN,i2c_SDA_PORT) & (1<<i2c_SDA_BIT))?0:1;
	_concat(DDR,i2c_SCL_PORT)|=(1<<i2c_SCL_BIT);
	return ret;
}

#ifdef i2c_PORT
#undef i2c_SDA_PORT
#undef i2c_SCL_PORT
#endif


#undef _concat
#undef __concat