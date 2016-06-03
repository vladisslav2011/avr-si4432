#include <stdint.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <string.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <avr/eeprom.h>   /* required by usbdrv.h */
#include "../si443x_regs.h"

#define i2cpfix st7558
#define i2c_delay() asm volatile ("nop;nop;nop; ")
//#define i2c_delay() _delay_us(2)

#define i2c_PORT C
#define i2c_SDA_BIT 4
#define i2c_SCL_BIT 5
#include "softI2C.h"
#undef i2cpfix
#undef i2c_delay
#undef i2c_PORT
#undef i2c_SDA_BIT
#undef i2c_SCL_BIT
#include "st7558.h"
#include "font.h"

#if defined (__AVR_ATmega8__)
#define SPI_SCK  5
#define SPI_MISO 4
#define SPI_MOSI 3
#define SPI_SS   2
#endif

#define LED0_ON() DDRD|=(1<<4)
#define LED0_OFF() DDRD&=~(1<<4)

#define LED1_TOGGLE() DDRC^=(1<<3)
#define LED1_ON() DDRC|=(1<<3)
#define LED1_OFF() DDRC&=~(1<<3)

#define BTN0_GET() ((PIND&(1<<7))==0)


#define GET_nIRQ() (PIND & (1<<3))

#define SPI_CS(x) if(x)PORTB|=(1<<PB2);else PORTB&=~(1<<PB2)
#define REGW_m(r,v) do{\
	SPI_CS(0);\
	SPDR=(r|0x80);\
	while((SPSR&(1<<SPIF))==0);\
	SPDR=(v);\
	while((SPSR&(1<<SPIF))==0);\
	SPI_CS(1);\
	\
	 }while(0)

void REGW(uint8_t r,uint8_t v)
{
	REGW_m(r,v);
}
 
#define REGR_m(r,v) do{\
	SPI_CS(0);\
	SPDR=(r&0x7f);\
	while((SPSR&(1<<SPIF))==0);\
	SPDR=0xff;\
	while((SPSR&(1<<SPIF))==0);\
	v=SPDR;\
	SPI_CS(1);\
	\
	 }while(0)

uint8_t REGR_f(uint8_t r)
{
	uint8_t res=0;
	REGR_m(r,res);
	return res;
}

#define REGR(r,v) v=REGR_f(r)

#define BURSTW(r,buf,n) do{\
	SPI_CS(0);\
	SPDR=(r|0x80);\
	while((SPSR&(1<<SPIF))==0);\
	uint8_t k;\
	for(k=0;k<n;k++)\
	{\
		SPDR=(buf[k]);\
		while((SPSR&(1<<SPIF))==0);\
	}\
	SPI_CS(1);\
	\
	 }while(0)

#define BURSTR(r,buf,n) do{\
	SPI_CS(0);\
	SPDR=(r&0x7f);\
	while((SPSR&(1<<SPIF))==0);\
	uint8_t k;\
	for(k=0;k<n;k++)\
	{\
		SPDR=0xff;\
		while((SPSR&(1<<SPIF))==0);\
		buf[k]=SPDR;\
	}\
	SPI_CS(1);\
	\
	 }while(0)

//		int fb=fr/10000000.0-24.0;
//		int fc=(fr/10000000.0-24.0-fb)*64000.0;

#define si4432_swreset() do{\
		REGW(0x07,0x80);\
		REGW(0x07,0x00);\
		REGW(0x07,0x01);\
		while(GET_nIRQ());\
		uint8_t dummy;\
		REGR(si_interrupt1,dummy);\
		REGR(si_interrupt2,dummy);\
		REGW(si_inten1,0);\
		REGW(si_inten2,0);\
	}while(0)


#define tune_base(fr) do{\
		REGW(0x75,(1<<6)|(((fr>=480000000.0)?1:0)<<5)|((uint8_t)(fr/10000000.0-24.0)));\
		REGW(0x76,(((uint32_t)((fr/10000000.0-24.0-((uint32_t)(fr/10000000.0-24.0)))*64000.0))>>8)&0xff);\
		REGW(0x77,((uint32_t)((fr/10000000.0-24.0-((uint32_t)(fr/10000000.0-24.0)))*64000.0))&0xff);\
	}while(0)

#define setupgpio(qoff) do{\
		REGW(0x0b,0x12);\
		REGW(0x0c,0x15);\
		REGW(0x0d,0x14);\
		REGW(0x0d,0x03);\
		REGW(0x09,qoff);\
		REGW(0x0a,0);\
	}while(0)


#define M_fd(de) (de*1000llu/625llu)
#define M_H(br,de,enmanch) (2llu*de/br/(1+enmanch))
#define M_BWmod(br,de,enmanch) (br*(1+enmanch)+2*de)
#define M_txdr(br) ((br<30000)?(br*(1llu<<21)/1000000llu):(br*(1llu<<16)/1000000llu))
#define M_slow(br) (br<30000)
#define M_iffbw(bandwidth,br,de,enmanch) si_get_iffbw(bandwidth,M_H(br,de,enmanch))
#define M_dwn3_bypass(bandwidth,br,de,enmanch) ((M_iffbw(bandwidth,br,de,enmanch)&0x80)>>7)
#define M_ndec_exp(bandwidth,br,de,enmanch) ((M_iffbw(bandwidth,br,de,enmanch)>>4)&0x7)
//#define M_Rb(br) (br/1000.0)

//#define M_rxosr(br,bandwidth,enmanch) (500.0*8.0*(1.0+2.0*M_dwn3_bypass(bandwidth))/((1llu<<M_ndec_exp(bandwidth))*M_Rb(br)*(1+enmanch)))
#define M_rxosr(br,bandwidth,de,enmanch) (4000000llu*(1+2*M_dwn3_bypass(bandwidth,br,de,enmanch))/\
	((1llu<<M_ndec_exp(bandwidth,br,de,enmanch))*br*(1+enmanch)))

//#define M_ncoff(bandwidth,br,de,enmanch) ((M_Rb(br)*(1llu<<(20+M_ndec_exp(bandwidth,br,de,enmanch))))/\
//	(500.0*(1+2*M_dwn3_bypass(bandwidth,br,de,enmanch))))
#define M_ncoff(bandwidth,br,de,enmanch) ((br*(1llu<<(20+M_ndec_exp(bandwidth,br,de,enmanch))))/\
	(500000llu*(1+2*M_dwn3_bypass(bandwidth,br,de,enmanch))))
#define M_crgain(br,de,bandwidth,enmanch) (2+(1llu<<15)*(1+enmanch)*br/1000/M_rxosr(br,bandwidth,de,enmanch)/de)

#define M_crgain_C(br,de,bandwidth,enmanch) ((M_crgain(br,de,bandwidth,enmanch)>0x07ff)?\
	(((uint32_t)M_crgain(br,de,bandwidth,enmanch)>>1)|0x8000):\
	(uint32_t)M_crgain(br,de,bandwidth,enmanch))
#define M_ncoff_C(bandwidth,br,de,enmanch) (((uint32_t)M_ncoff(bandwidth,br,de,enmanch)>=(1llu<<20))?\
	((1llu<<20)-1):M_ncoff(bandwidth,br,de,enmanch))
#define M_afc_bw_lim(bandwidth,br,de,enmanch) (1000llu*bandwidth/1250llu/(0+1))
#define M_afc_bw_lim_C(bandwidth,br,de,enmanch) ((M_afc_bw_lim(bandwidth,br,de,enmanch)>80)?80:M_afc_bw_lim(bandwidth,br,de,enmanch))


#define setup_modem(br,deviation,bandwidth,enmanch,enafc) do{\
		uint8_t buf;\
		REGR(0x70,buf);\
		if(enmanch)buf|=0x02;else buf&=~0x02;\
		buf&=~(1<<5);\
		buf|=(M_slow(br)<<5);\
		REGW(0x70,buf);\
		REGR(0x71,buf);\
		buf&=~(1<<2);\
		buf|=((M_fd(deviation)>>(8-2))&(1<<2));\
		REGW(0x71,buf);\
		REGW(0x72,M_fd(deviation)&0xff);\
		REGW(0x6f,M_txdr(br)&0xff);\
		REGW(0x6e,(M_txdr(br)>>8)&0xff);\
		REGW(0x1c,M_iffbw(bandwidth,br,deviation,enmanch));\
		REGW(0x20,(uint32_t)M_rxosr(br,bandwidth,deviation,enmanch)&0xff);\
		REGW(0x21,(((uint32_t)M_rxosr(br,bandwidth,deviation,enmanch)>>3)&0xe0)|\
			(((uint32_t)M_ncoff_C(bandwidth,br,deviation,enmanch)>>16)&0x0f));\
		REGW(0x22,((uint32_t)M_ncoff_C(bandwidth,br,deviation,enmanch)>>8)&0xff);\
		REGW(0x23,(uint32_t)M_ncoff_C(bandwidth,br,deviation,enmanch)&0xff);\
		REGW(0x24,(M_crgain_C(br,deviation,bandwidth,enmanch)>>8)&0x0f);\
		REGW(0x25,M_crgain_C(br,deviation,bandwidth,enmanch)&0xff);\
		REGW(0x1d,enafc?(/*si_afcbd|*/si_enafc|si_1p5bypass):0);\
		REGW(0x2a,enafc?M_afc_bw_lim_C(bandwidth,br,deviation,enmanch):0x00);\
	}while(0)
	

struct modem_regs
{
	uint8_t reg1c;
	uint8_t reg1d;
	uint8_t reg20;
	uint8_t reg21;
	uint8_t reg22;
	uint8_t reg23;
	uint8_t reg24;
	uint8_t reg25;
	uint8_t reg2a;
	uint8_t reg2b;
	uint8_t reg2c;
	uint8_t reg2d;
	uint8_t reg2e;
	uint8_t reg6d;
	uint8_t reg6e;
	uint8_t reg6f;
	uint8_t reg70;
	uint8_t reg71;
	uint8_t reg72;
};
#if 1
uint8_t modem_saved_regs[sizeof(struct modem_regs)] PROGMEM=
{
	0x1c,
	0x1d,
	0x20,
	0x21,
	0x22,
	0x23,
	0x24,
	0x25,
	0x2a,
	0x2b,
	0x2c,
	0x2d,
	0x2e,
	0x6d,
	0x6e,
	0x6f,
	0x70,
	0x71,
	0x72,
};
#endif
void save_modem(struct modem_regs * buffer)
{
#if 0
	REGR(0x1c,buffer->reg1c);
	REGR(0x1d,buffer->reg1d);
	REGR(0x20,buffer->reg20);
	REGR(0x21,buffer->reg21);
	REGR(0x22,buffer->reg22);
	REGR(0x23,buffer->reg23);
	REGR(0x24,buffer->reg24);
	REGR(0x25,buffer->reg25);
	REGR(0x2a,buffer->reg2a);
	REGR(0x2b,buffer->reg2b);
	REGR(0x2c,buffer->reg2c);
	REGR(0x2d,buffer->reg2d);
	REGR(0x2e,buffer->reg2e);
	REGR(0x6d,buffer->reg6d);
	REGR(0x6e,buffer->reg6e);
	REGR(0x6f,buffer->reg6f);
	REGR(0x70,buffer->reg70);
	REGR(0x71,buffer->reg71);
	REGR(0x72,buffer->reg72);
#else
	uint8_t k;
	uint8_t * pp=modem_saved_regs;
	uint8_t * pb=(uint8_t *)buffer;
	for(k=0;k<sizeof(struct modem_regs);k++,pp++,pb++)
		REGR(pgm_read_byte(pp),*pb);
#endif
}

void restore_modem(struct modem_regs * buffer)
{
#if 0
	REGW(0x1c,buffer->reg1c);
	REGW(0x1d,buffer->reg1d);
	REGW(0x20,buffer->reg20);
	REGW(0x21,buffer->reg21);
	REGW(0x22,buffer->reg22);
	REGW(0x23,buffer->reg23);
	REGW(0x24,buffer->reg24);
	REGW(0x25,buffer->reg25);
	REGW(0x2a,buffer->reg2a);
	REGW(0x2b,buffer->reg2b);
	REGW(0x2c,buffer->reg2c);
	REGW(0x2d,buffer->reg2d);
	REGW(0x2e,buffer->reg2e);
	REGW(0x6d,buffer->reg6d);
	REGW(0x6e,buffer->reg6e);
	REGW(0x6f,buffer->reg6f);
	REGW(0x70,buffer->reg70);
	REGW(0x71,buffer->reg71);
	REGW(0x72,buffer->reg72);
#else
	uint8_t k;
	uint8_t * pp=modem_saved_regs;
	uint8_t * pb=(uint8_t *)buffer;
	for(k=0;k<sizeof(struct modem_regs);k++,pp++,pb++)
		REGW(pgm_read_byte(pp),*pb);
#endif
}
/*
*/

#define FL_OVF2   0x01
#define FL_OC2    0x02
#define FL_CONTRX 0x04
#define FL_INT    0x08
#define FL_SAVED  0x10
#define FL_PREA   0x20


struct softclock
{
	uint8_t sec;
	uint8_t min;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint16_t year;
	uint8_t running;
};

volatile struct softclock rtc;
volatile uint32_t counter;
volatile uint32_t comp;
volatile uint8_t flags;

/* ------------------------------------------------------------------------- */
void init_timer1()
{
	TCNT1=0;
	OCR1B=0;
	TCCR1A=0;
	TCCR1B=(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
	TIMSK|=(1<<TOIE1)|(0<<OCIE1A)|(0<<TICIE1)|(0<<OCIE1B);
	ACSR=(1<<ACD);
	counter=0;
	
}

/* ------------------------------------------------------------------------- */
void init_timer2()
{
	ASSR=(1<<AS2);
	TCNT2=0;
	OCR2=0;
	TCCR2=(1<<CS22)|(0<<CS22)|(1<<CS22);
	TIMSK|=(1<<TOIE2)|(1<<OCIE2);
	ACSR=(1<<ACD);
	counter=0;
	while(TCNT2==0);
	
}


/* ------------------------------------------------------------------------- */
ISR(SIG_OVERFLOW1)
{
	++counter;
}

/* ------------------------------------------------------------------------- */
ISR(SIG_OVERFLOW2)
{
	++counter;
	if(counter&0x01)
		if(rtc.running)
		{
			++rtc.sec;
			if(rtc.sec>=60)
			{
				rtc.sec=0;
				++rtc.min;
				if(rtc.min>=60)
				{
					++rtc.hour;
					rtc.min=0;
					if(rtc.hour>=24)
					{
						++rtc.day;
						rtc.hour=0;
						uint8_t monch=0;
						switch(rtc.month)
						{
						case 1:
							if((rtc.year&0x03)==0)
							{
								if(rtc.day>=29)
								{
									rtc.day=0;
									rtc.month++;
									monch=1;
								}
							}else{
								if(rtc.day>=28)
								{
									rtc.day=0;
									rtc.month++;
									monch=1;
								}
							}
							break;
						case 0:
						case 2:
						case 4:
						case 6:
						case 7:
						case 9:
						case 11:
							if(rtc.day>=31)
							{
								rtc.day=0;
								rtc.month++;
								monch=1;
							};
							break;
						default:
							if(rtc.day>=30)
							{
								rtc.day=0;
								rtc.month++;
								monch=1;
							};
						};
						if(monch)
						{
							if(rtc.month>=11)
							{
								rtc.month=0;
								rtc.year++;
							}
						}
					};
				};
			};
			
		}
	flags|=FL_OVF2;
}

/* ------------------------------------------------------------------------- */
ISR(SIG_OUTPUT_COMPARE2)
{
	++comp;
	flags|=FL_OC2;
}

void init_spi()
{
	DDRB=(1<<PB2)|(1<<PB3)|(1<<PB5);
	PORTB=(1<<PB4);
	//PORTB=(1<<PB0)|(1<<PB1)|(1<<PB4);
	SPCR=(0<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR0)|(0<<SPR1);
	SPCR=(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(0<<SPR0)|(0<<SPR1);
	SPSR=(0<<SPI2X);
}

uint16_t read_adc()
{
	ADMUX=(1<<REFS0)|0x0e;
	ADCSRA=(1<<ADEN)|0x07;
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	ADCSRA|=(1<<ADSC);
	while(ADCSRA&(1<<ADSC));
	uint16_t ret=ADC;
	ADMUX=0;
	ADCSRA=0;
	return ret;
}

uint8_t st7558_prchar(uint8_t c)
{
	if(c>=sizeof(charmap)/8)
		return 1;
	uint8_t k;
	uint8_t * cp=&charmap[c*8];
	st7558_startstr();
	for(k=0;k<8;k++)
		st7558_write(pgm_read_byte(cp++));
	st7558_i2cSTOP();
	return 0;
}

void st7558_prstr(const uint8_t * S)
{
	uint8_t * p=(uint8_t *)S;
	while(st7558_prchar(pgm_read_byte(p++))==0)
		;
}

uint8_t st7558_prxchar(uint8_t x)
{
	if(x>=10)
		return st7558_prchar(x-10+VC_A);
	else
		return st7558_prchar(x);
}

void st7558_print(uint32_t in,uint8_t min)
{
	uint8_t buf[10];
	uint8_t bp=0;
	while(in)
	{
		buf[bp++]=in%10;
		in/=10;
	}
	if(bp==0)
		buf[bp++]=0;
	while(bp<min)
		buf[bp++]=VC_SPACE;
	while(bp)
	{
		bp--;
		st7558_prchar(buf[bp]);
	}
}

void st7558_prhex(uint32_t in,uint8_t bp)
{
	while(bp)
	{
		bp--;
		st7558_prxchar( ((uint8_t*)&in)[bp]>>4);
		st7558_prxchar( ((uint8_t*)&in)[bp]&0x0f);
	}
}



static uint8_t rssib[96];
static uint8_t rssif[96];
static uint8_t prssi;

#define GR_TOP 4
void pr_rssi()
{
	uint8_t k,t,p;
	uint8_t m[4]={24,16,8,0};
	uint8_t l[4]={32,24,16,8};
		//uint8_t trssi=(((uint16_t)(rssib[k]-25))*4)/25;
	for(t=0;t<4;t++)
	{
		st7558_setpos(0,GR_TOP+t);
		st7558_startstr();
		for(k=0,p=prssi;k<96;k++)
		{
			uint8_t d=0;
			if(rssib[p]>m[t])
				d=(rssib[p]>=l[t])?0xff:(0xff<<(l[t]-rssib[p]));
			else
				d=0;
			if(rssif[p]>m[t])
				d&=~((rssif[p]>=l[t])?0xff:(0xff<<(l[t]-rssif[p])));
			st7558_write(d);
			p++;
			if(p==sizeof(rssib))
				p=0;
		}
		st7558_i2cSTOP();
	}
}



struct pkt_in
{
	uint8_t cmd;
	uint8_t arg0;
	uint8_t arg1;
	uint8_t arg2;
	uint32_t seq;
}




volatile uint8_t int1;
volatile uint8_t int2;
volatile uint8_t trssi;
volatile uint8_t frssi;
volatile uint8_t tagc;
volatile struct pkt_in ipkt;
volatile uint32_t validpkts;
volatile uint16_t afc_corr;
struct modem_regs reg_backup;
struct modem_regs reg_new;

/* ------------------------------------------------------------------------- */
ISR(SIG_INTERRUPT1)
{
	uint8_t tmp1,tmp2;
//	GICR&=~(1<<INT1);
	LED0_ON();
	REGR(si_interrupt1,int1);
	REGR(si_interrupt2,int2);
//	GIFR=0xff;
	if(int2&si_ipreaval)
	{
		REGR(si_rssi,trssi);
		REGR(si_agc_override,tagc);
		tmp1=trssi/7;
		if(rssib[prssi]<tmp1)
		{
			rssib[prssi]=tmp1;
		}
	}
	if(int1&si_ipkvalid)
	{
		BURSTR(si_fifo,((uint8_t *)&ipkt),sizeof(ipkt));
		REGR(si_rssi,frssi);
		REGR(si_afccorrh,tmp1);
		REGR(si_ookcnt1,tmp2);
		afc_corr=(tmp2>>si_afccorrl_shift)|(tmp1<<2);
		rssif[prssi]=frssi/7;
		prssi++;
		if(prssi==sizeof(rssib))
			prssi=0;
		rssif[prssi]=rssib[prssi]=0;
		validpkts++;
		flags|=FL_INT;
	}
	LED0_OFF();
}

void set_modem_conf(uint8_t conf)
{
	switch(conf)
	{
	case 0x01:	setup_modem(2400,2,7,0,1);break;
	case 0x02:	setup_modem(4800,5,15,0,1);break;
	case 0x03:	setup_modem(9600,5,22,0,1);break;
	case 0x04:	setup_modem(19200,15,50,0,1);break;
	case 0x05:	setup_modem(38400,50,150,0,1);break;
	case 0x06:	setup_modem(50000,50,160,0,1);break;
	case 0x07:	setup_modem(100000,100,300,0,1);break;
	case 0x08:	setup_modem(200000,200,600,0,1);break;
	default:	setup_modem(1200,1,3,0,1);break;
	}
}
void set_pwr(uint8_t pwr)
{
	REGW(0x6d,0x18|(pwr&0x07));
}




void main(void)
{
	validpkts=0;
	LED1_ON();
	PORTD|=(1<<3)|(1<<7);
	//PORTC|=(1<<3);
	init_spi();
	init_timer2();
    sei();
	st7558_init();
	st7558_setpos(0,0);
	
	memset(rssib,0,sizeof(rssib));
	memset(rssif,0,sizeof(rssif));
	uint8_t k;
	for(k=33;k!=255;k--)
		rssib[33-k]=k;
	for(k=0;k!=33;k++)
		rssib[k+33]=k;
	for(k=33;k!=255;k--)
		rssif[33-k]=k-1;
	for(k=0;k!=33;k++)
		rssif[k+33]=k-1;
	pr_rssi();
	
	si4432_swreset();
	setupgpio(0x53);
	REGW(si_modcon2,si_dtmod_fifo|si_modtyp_gfsk);
	//setup_modem(50000,50,180,0,1);
//	uint8_t selbr=eeprom_read_byte(0);
	set_modem_conf(0);
	tune_base(433920000);
	REGW(si_headcon1,0);
	REGW(si_headcon2,si_fixpklen|si_syncword3210);
//	REGW(si_headcon2,0);
	REGW(si_prealenl,32);
	REGW(si_sync3,0x0f);
	REGW(si_sync2,0x34);
	REGW(si_sync1,0x21);
	REGW(si_sync0,0xff);
	REGW(si_data_access_contorl,si_enpacrx|si_enpactx|si_encrc|si_crc16);
	REGW(si_inten1,0);
	REGW(si_inten2,0);
	REGW(si_mode02,si_ffclrrx|si_ffclrtx);
	REGW(si_mode02,0);
	REGW(si_mode02,si_rxmpk);
	REGW(si_inten1,si_ipkvalid);
	REGW(si_inten2,si_ipreaval);
	REGR(si_interrupt1,int1);
	REGR(si_interrupt2,int2);
//	REGR(0x72,pkt.fill);
	struct modem_regs saved;
	save_modem(&saved);
	restore_modem(&saved);
	LED1_OFF();
	
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	GIFR=0xff;
	GICR|=(1<<INT1);
	REGW(si_pklen,sizeof(ipkt));
	REGW(si_mode01,si_rxon);
	uint32_t ipkt_s=0;
	uint8_t ipkt_s_got=0;
	uint8_t btn_prev=0;
	
    for(;;){                /* main event loop */
//		LED0_OFF();
		if(flags&FL_OC2)
		{
			flags&=~FL_OC2;
			OCR2=TCNT2+16;
			uint8_t btn_tmp=BTN0_GET();
			if(btn_tmp!=btn_prev)
			{
				btn_prev=btn_tmp;
				if(btn_prev)
					LED1_TOGGLE();
			}
		}
		if(flags&FL_OVF2)
		{
			flags&=~FL_OVF2;
/*			st7558_setpos(0,3);
			st7558_print(counter&0xffff,5);*/
		}
		if(flags&FL_INT)
		{
			LED0_OFF();
			flags&=~FL_INT;
				pr_rssi();
				st7558_setpos(0,0);
				st7558_prchar(VC_D);
				st7558_prchar(VC_SPACE);
				uint32_t tipkt;
				((uint8_t*)&tipkt)[0]=((uint8_t *)&ipkt)[3];
				((uint8_t*)&tipkt)[1]=((uint8_t *)&ipkt)[2];
				((uint8_t*)&tipkt)[2]=((uint8_t *)&ipkt)[1];
				((uint8_t*)&tipkt)[3]=((uint8_t *)&ipkt)[0];
				if(!ipkt_s_got)
				{
					ipkt_s_got=1;
					ipkt_s=tipkt;
				}
				st7558_print(tipkt-ipkt_s,10);
				st7558_setpos(0,1);
				st7558_prchar(VC_N);
				st7558_prchar(VC_SPACE);
				st7558_print(validpkts,10);
				st7558_setpos(0,2);
				st7558_prchar(VC_R);
				st7558_prchar(VC_S);
				st7558_prchar(VC_S);
				st7558_prchar(VC_I);
				st7558_prchar(VC_EQ);
				st7558_print(trssi,3);
				st7558_prchar(VC_COMMA);
				st7558_print(frssi,3);
				st7558_setpos(0,3);
				st7558_prhex(tagc,1);
				st7558_setpos(6*8,3);
				st7558_print(afc_corr,4);
		}
/*		if(flags&FL_CONTRX)
		{
			REGW(si_pklen,sizeof(ipkt));
			REGW(si_mode02,si_ffclrrx);
			REGW(si_mode02,0);
			REGW(si_mode01,si_rxon);
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			REGW(si_inten1,si_ipkvalid);
			REGW(si_inten2,si_ipreaval);
			TIMSK&=~(1<<OCIE2);
			TIFR=0xff;
			flags&=(1<<FL_OVF2);
			GIFR=0xff;
			GICR|=(1<<INT1);
			LED0_OFF();
			flags&=~FL_CONTRX;
		}*/
		//GIFR=0xff;
		//GICR|=(1<<INT1);
		if((flags&(FL_INT))==0)
		{
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
    };
}
