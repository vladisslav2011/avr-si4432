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
#define i2c_delay() _delay_us(2)

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




struct modem_regs
{
	uint8_t reg1c;
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

uint8_t modem_saved_regs[sizeof(struct modem_regs)] PROGMEM=
{
	0x1c,
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

void save_modem(struct modem_regs * buffer)
{
	REGR(0x1c,buffer->reg1c);
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
}

void restore_modem(struct modem_regs * buffer)
{
	REGW(0x1c,buffer->reg1c);
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
}
/*
*/

#define FL_OVF2   0x01
#define FL_OC2    0x02
#define FL_CONTRX 0x04
#define FL_INT    0x08
#define FL_SAVED  0x10


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
	TIMSK|=(1<<TOIE2);
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

/* ------------------------------------------------------------------------- */
ISR(SIG_INTERRUPT1)
{
	flags|=FL_INT;
	GICR&=~(1<<INT1);
	GIFR=0xff;
	LED0_ON();
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
	for(k=0;k<8;k++)
		st7558_putbyte(pgm_read_byte(cp++));
	return 0;
}

void st7558_prstr(const uint8_t * S)
{
	uint8_t * p=(uint8_t *)S;
	while(st7558_prchar(pgm_read_byte(p++))==0)
		;
}




struct config
{
	uint8_t txperiod;
	uint8_t replydelay;
	uint8_t rxduration;
	uint8_t restore_time;
};


struct pkt_default{
	struct softclock rtc;
	uint32_t cc;
	uint8_t type;
	uint8_t temp;
	uint16_t vcc;
};

struct pkt_getreg{
	struct softclock rtc;
	uint32_t cc;
	uint8_t type;
	uint8_t reg;
	uint8_t data;
	uint8_t fill;
};

union pkt_u {
	struct pkt_default def;
	struct pkt_getreg getreg;
};

#define RFCMD_TXPERIOD     0
#define RFCMD_CONTRX       1
#define RFCMD_REPLYDELAY   2
#define RFCMD_RXDURATION   3
#define RFCMD_SETREG       4
#define RFCMD_GETREG       5
#define RFCMD_SETCLOCK     6

struct inpkt_common{
	uint8_t type;
	uint8_t fill0;
	uint8_t fill1;
	uint8_t fill2;
};

struct inpkt_set_txperiod{
	uint8_t type;
	uint8_t period;
	uint16_t fill;
};

struct inpkt_set_contrx{
	uint8_t type;
	uint8_t contrx;
	uint16_t fill;
};

struct inpkt_set_replydelay{
	uint8_t type;
	uint8_t delay;
	uint16_t fill;
};
struct inpkt_set_rxduration{
	uint8_t type;
	uint8_t dur;
	uint16_t fill;
};

struct inpkt_get_reg{
	uint8_t type;
	uint8_t reg;
	uint8_t latch;
	uint8_t save;
};
struct inpkt_set_reg{
	uint8_t type;
	uint8_t reg;
	uint8_t val;
	uint8_t commit;
};
struct inpkt_set_clock{
	uint8_t type;
	uint8_t byte;
	uint8_t val;
	uint8_t fill;
};

union ipkt_u {
	struct inpkt_common common;
	struct inpkt_set_txperiod txperiod;
	struct inpkt_set_contrx contrx;
	struct inpkt_set_replydelay replydelay;
	struct inpkt_set_rxduration rxduration;
	struct inpkt_get_reg get_reg;
	struct inpkt_set_reg set_reg;
	struct inpkt_set_clock set_clock;
};





static union pkt_u pkt;
static union ipkt_u ipkt;
static uint8_t tmp;
static uint8_t int1;
static uint8_t int2;
struct config conf;
struct modem_regs reg_backup;
struct modem_regs reg_new;


void main(void)
{
	pkt.def.cc=0;
	pkt.def.type=0;
	pkt.def.vcc=0x00;
	
	conf.txperiod=4;
	conf.replydelay=16;
	conf.rxduration=160;
	conf.restore_time=2*30;//30 sec to test modem settings
//	flags=FL_CONTRX;
	DDRD|=(1<<4);
	PORTD|=(1<<3);
	init_spi();
	init_timer2();
    sei();
/*	st7558_init();
	st7558_setpos(0,0);
	uint8_t sk;
	for(sk=0;sk!=20;sk++)
		st7558_prchar(sk);*/
	
	
	si4432_swreset();
/*	REGW(0x5a,0x03);
	REGW(0x59,0x00);
	REGW(0x66,0x02);*/
	si4432_setupgpio(si_gpio0txstate,si_gpio1rxstate,si_gpio2rxdat,0x58);
/*	REGW(si_modcon2,si_dtmod_fifo|si_modtyp_gfsk);
	setup_modem(50000,50,200,0,1,si_modtyp_gfsk);*/
	REGW(si_modcon2,si_dtmod_fifo|si_modtyp_ook);
	si4432_setup_modem(5000,50,120,0,1,si_modtyp_ook);
	si4432_tune_base(433920000);
	si4432_hop(0,0);
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
	REGW(si_mode01,0);
//	REGR(0x72,pkt.fill);
	DDRD&=~(1<<4);
	
	set_sleep_mode(SLEEP_MODE_PWR_SAVE);
	uint8_t dotx=0;
	cli();
	uint32_t next_tx=counter+conf.txperiod;
	uint16_t restore=0;
	sei();
	
    for(;;){                /* main event loop */
//		LED0_OFF();
		if((flags&(FL_OVF2|FL_OC2|FL_INT))==0)
		{
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
//		LED0_ON();
		uint32_t lcounter=counter;
		if(flags&FL_OVF2)
			if(restore)
			{
				restore--;
				if(!restore)
					restore_modem(&reg_backup);
			}
		if(flags&FL_OC2)
		{
			flags&=~FL_OC2;
			if(dotx==0)
				if(!(flags&FL_CONTRX))
				{
					REGW(si_inten1,0);
					REGW(si_inten2,0);
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					REGW(si_mode01,0);
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					TIMSK&=~(1<<OCIE2);
					TIFR=0xff;
					flags&=(1<<FL_OVF2);
					continue;
				}
			TIMSK&=~(1<<OCIE2);
			TIFR=0xff;
			flags&=(1<<FL_OVF2);
		}
		if(flags&FL_INT)
		{
			flags&=~FL_INT;
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			if(int1&si_ipkvalid)
			{
/*				uint8_t rxplen=0;
				REGR(si_rxplen,rxplen);
				if(rxplen!=sizeof(ipkt))
				{
					REGW(si_mode02,si_ffclrrx);
					REGW(si_mode02,0);
				}else{*/
					BURSTR(si_fifo,((uint8_t *)&ipkt),sizeof(ipkt));
					LED0_OFF();
					if(restore)//reset restore if rx still works
						restore=conf.restore_time;
					switch(ipkt.common.type)
					{
					case RFCMD_TXPERIOD:
						conf.txperiod=ipkt.txperiod.period;
					break;
					case RFCMD_CONTRX:
						if(ipkt.contrx.contrx)
							flags|=FL_CONTRX;
						else
							flags&=~FL_CONTRX;
					break;
					case RFCMD_REPLYDELAY:
						conf.replydelay=ipkt.replydelay.delay;
					break;
					case RFCMD_RXDURATION:
						conf.rxduration=ipkt.rxduration.dur;
					break;
					case RFCMD_SETREG:
						do{
							if(!(flags&FL_SAVED))
							{
								save_modem(&reg_backup);
								save_modem(&reg_new);
								flags|=FL_SAVED;
							}
							uint8_t k=0;
							for(k=0;k<sizeof(struct modem_regs);k++)
								if(pgm_read_byte(&modem_saved_regs[k])==ipkt.set_reg.reg)
									break;
							if(k<sizeof(struct modem_regs))
							{
								((uint8_t *)&reg_new)[k]=ipkt.set_reg.val;
								if(ipkt.set_reg.commit)
								{
									restore_modem(&reg_new);
									restore=conf.restore_time;
								}
							}else{
								REGW(ipkt.set_reg.reg,ipkt.set_reg.val);
							}
							
						}while(0);
					break;
					case RFCMD_GETREG:
						pkt.getreg.type=1;
						pkt.getreg.reg=ipkt.get_reg.reg;
						REGR(ipkt.get_reg.reg,pkt.getreg.data);
						dotx=1;
						LED0_ON();
						if(ipkt.get_reg.save)
						{
							save_modem(&reg_backup);
							save_modem(&reg_new);
							flags|=FL_SAVED;
						}
						if(conf.replydelay>=1)
						{
							OCR2=TCNT2+conf.replydelay;
							while(ASSR&0x07);
							TIFR=0xff;
							flags&=(1<<FL_OVF2);
							TIMSK|=(1<<OCIE2);
						}
					break;
					case RFCMD_SETCLOCK:
						if(ipkt.set_clock.byte>=sizeof(struct softclock))
							break;
						cli();
						((uint8_t *)&rtc)[ipkt.set_clock.byte]=ipkt.set_clock.val;
						sei();
					break;
					}
//				}
				if(conf.replydelay>=1)
					if(dotx)
						continue;
			}
			if(!dotx)
			{
				if(flags&FL_CONTRX)
				{
					REGW(si_pklen,sizeof(ipkt));
					REGW(si_mode02,si_ffclrrx);
					REGW(si_mode02,0);
					REGW(si_mode01,si_rxon);
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					REGW(si_inten1,si_ipkvalid);
					REGW(si_inten2,0);
					TIMSK&=~(1<<OCIE2);
					TIFR=0xff;
					flags&=(1<<FL_OVF2);
					GIFR=0xff;
					GICR|=(1<<INT1);
				}else{
					REGW(si_inten1,0);
					REGW(si_inten2,0);
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					REGW(si_mode01,0);
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					TIMSK&=~(1<<OCIE2);
					TIFR=0xff;
					flags&=(1<<FL_OVF2);
				}
			}
		}
		if(flags & FL_OVF2)
		{
			flags&=~FL_OVF2;
			cli();
			memcpy(&pkt.def.rtc,&rtc,sizeof(rtc));
			sei();
			pkt.def.rtc.month++;
			pkt.def.rtc.day++;
			if(!dotx)//if we already have a packet to transmit, skip periodic
			{
				if(conf.txperiod==0)//force continous rx if periodic is disabled
				{
					flags|=FL_CONTRX;
					REGW(si_pklen,sizeof(ipkt));
					REGW(si_mode02,si_ffclrrx);
					REGW(si_mode02,0);
					REGW(si_mode01,si_rxon);
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					REGW(si_inten1,si_ipkvalid);
					REGW(si_inten2,0);
					TIMSK&=~(1<<OCIE2);
					TIFR=0xff;
					GIFR=0xff;
					GICR|=(1<<INT1);
				}else
					if(lcounter>=next_tx)
					{
						//tx default packet
						next_tx=lcounter+conf.txperiod;
						++pkt.def.cc;
						REGW(si_adc_con,si_adcstart);
						do{
							REGR(si_adc_con,tmp);
							if(tmp&si_adcstart);
								break;
						}while(1);
						REGW(si_adc_con,0);
						REGR(si_adc,pkt.def.temp);
						pkt.def.vcc=read_adc();
						pkt.def.type=0;
						dotx=1;
					}
			}
		}
		if(dotx)
		{
			dotx=0;
			REGW(si_pklen,sizeof(pkt));
			BURSTW(si_fifo,((uint8_t *)&pkt),sizeof(pkt));
/*			struct modem_regs rr;
			save_modem(&rr);
			REGW(si_pklen,sizeof(rr));
			BURSTW(si_fifo,((uint8_t *)&rr),sizeof(rr));*/
			REGW(si_inten1,si_ipksent);
			REGW(si_inten2,0);
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			REGW(si_mode01,si_pllon|si_xton);
			GICR&=~(1<<INT1);
			REGW(si_mode01,si_txon);
			while(GET_nIRQ());
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			REGW(si_inten1,0);
			REGW(si_inten2,0);
			do{
				REGR(si_status,tmp);
			}while((tmp&si_cps_mask)!=si_cps_idle);
			REGW(si_pklen,sizeof(ipkt));
			REGW(si_mode02,si_ffclrrx);
			REGW(si_mode02,0);
			REGW(si_mode01,si_rxon);
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			REGW(si_inten1,si_ipkvalid);
			REGW(si_inten2,0);
			if(flags&FL_CONTRX)
			{
				TIMSK&=~(1<<OCIE2);
				TIFR=0xff;
				flags&=(1<<FL_OVF2);
			}else{
				OCR2=TCNT2+conf.rxduration;
				while(ASSR&0x07);
				flags&=(1<<FL_OVF2);
				TIFR=0xff;
				TIMSK|=(1<<OCIE2);
			}
			GIFR=0xff;
			GICR|=(1<<INT1);
			LED0_OFF();
		}
    };
}
