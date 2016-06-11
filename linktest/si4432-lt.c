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
#define i2c_delay() asm volatile ("nop;\n\tnop;\n\tnop; ")
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
void REGWI(uint8_t r,uint8_t v)
{
	cli();
	REGW_m(r,v);
	sei();
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
#define REGRI(r,v) do{cli();v=REGR_f(r);sei();}while(0)

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


#define mode01_def (si_enlbd)

#define FL_OVF2   0x01
#define FL_OC2    0x02
#define FL_CONTRX 0x04
#define FL_INT    0x08
#define FL_SAVED  0x10
#define FL_PREA   0x20
#define FL_REFR   0x40

union WD
{
	uint16_t w[2];
	uint32_t d;
};

volatile uint16_t counter;
volatile uint8_t flags;
volatile uint8_t nextmode;
volatile union WD latch1,delta;
union WD cal1,cal2;
volatile uint8_t txpw;

/* ------------------------------------------------------------------------- */
void init_timer1()
{
	TCNT1=0;
	OCR1B=0;
	TCCR1A=0;
	TCCR1B=(0<<WGM12)|(0<<CS12)|(0<<CS11)|(1<<CS10);
	TIMSK|=(1<<TOIE1)|(0<<OCIE1A)|(0<<TICIE1)|(0<<OCIE1B);
	
}

/* ------------------------------------------------------------------------- */
void init_timer2()
{
	ASSR=(1<<AS2);
	TCNT2=0;
	OCR2=0;
	TCCR2=(0<<CS22)|(1<<CS21)|(1<<CS20);
	TIMSK|=(1<<TOIE2)|(0<<OCIE2);
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
#define BTN_DUR 2
ISR(SIG_OVERFLOW2)
{
/*	static uint8_t comp;
	++comp;
	if(!(comp&7))
	{
		flags|=FL_OVF2;
		LED0_ON();
	}*/
	//flags|=FL_OC2;
	cal2.w[0]=TCNT1;
	cal2.w[1]=counter;
	flags|=FL_OVF2;
	LED0_ON();
	static uint8_t btn_prev=0;
	static uint8_t btn_dur=0;
	uint8_t btn_tmp=BTN0_GET();
	if(btn_tmp)
	{
		if(btn_dur<BTN_DUR)
			btn_dur++;
	}
	if(btn_dur==BTN_DUR)
	{
		LED1_TOGGLE();
		btn_dur++;
	}
	if(btn_tmp!=btn_prev)
	{
		btn_prev=btn_tmp;
		if(!btn_prev)
		{
			if(btn_dur<BTN_DUR)
//				nextmode++;
				txpw++;
			btn_dur=BTN_DUR+2;
		}else
			btn_dur=0;
	}
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
	uint8_t arg0;
	uint8_t arg1;
};




volatile uint8_t int1;
volatile uint8_t int2;
volatile uint8_t trssi;
volatile uint8_t frssi;
volatile uint8_t tagc;
volatile struct pkt_in ipkt;
struct pkt_in opkt;
volatile uint32_t validpkts;
volatile uint16_t afc_corr;
struct modem_regs reg_backup;
struct modem_regs reg_new;

/* ------------------------------------------------------------------------- */
ISR(SIG_INPUT_CAPTURE1)
{
	uint8_t tmp1,tmp2;
	delta.w[0]=ICR1;
	delta.w[1]=counter;
	delta.d-=latch1.d;
/*	TIMSK|=(1<<TOIE2);
	TIMSK&=~(1<<OCIE1A);
	TIFR=(1<<OCIE1A);*/
	REGR(si_status,tmp1);
	if(!(tmp1&si_rxffem))
	{
		BURSTR(si_fifo,((uint8_t *)&ipkt),sizeof(ipkt));
		REGR(si_afccorrh,tmp1);
		REGR(si_ookcnt1,tmp2);
		REGR(si_interrupt1,int1);
		REGR(si_interrupt2,int2);
		afc_corr=(tmp2>>si_afccorrl_shift)|(tmp1<<2);
		validpkts++;
	}
	flags|=FL_INT;
	LED0_OFF();
}
/* ------------------------------------------------------------------------- */
ISR(SIG_INTERRUPT1)
{
//	uint8_t tmp1;
//	GICR&=~(1<<INT1);
	REGR(si_interrupt1,int1);
	REGR(si_interrupt2,int2);
//	GIFR=0xff;
	if(int1&si_ipksent)
	{
		latch1.w[0]=TCNT1;
		latch1.w[1]=counter;
/*		OCR1A=latch1+60000;
 		TIMSK|=(1<<OCIE1A);*/
		REGW(si_mode01,mode01_def|si_rxon|si_pllon|si_xton);
		REGR(si_rssi,frssi);
		trssi=frssi+7;
	}
	if(int2&si_ipreaval)
	{
		REGR(si_rssi,trssi);
//  		TIMSK&=~(1<<TOIE2);
	}
}

/* ------------------------------------------------------------------------- */
ISR(SIG_OUTPUT_COMPARE1A)
{
	TIMSK&=~(1<<OCIE1A);
	TIMSK|=(1<<TOIE2);
	REGW(si_mode01,mode01_def);
	LED0_OFF();
}


void set_modem_conf(uint8_t conf)
{
	switch(conf)
	{
	case 0x01:	si4432_setup_modem(2400,2,7,0,1,si_modtyp_fsk);break;
	case 0x02:	si4432_setup_modem(4800,5,15,0,1,si_modtyp_fsk);break;
	case 0x03:	si4432_setup_modem(9600,5,22,0,1,si_modtyp_fsk);break;
	case 0x04:	si4432_setup_modem(19200,15,55,0,1,si_modtyp_fsk);break;
	case 0x05:	si4432_setup_modem(38400,50,142,0,1,si_modtyp_fsk);break;
	case 0x06:	si4432_setup_modem(50000,50,160,0,1,si_modtyp_fsk);break;
	case 0x07:	si4432_setup_modem(100000,100,300,0,1,si_modtyp_fsk);break;
	case 0x08:	si4432_setup_modem(200000,200,600,0,1,si_modtyp_fsk);break;
	
	default:	si4432_setup_modem(1200,1,4,0,1,si_modtyp_fsk);break;
	}
}


uint8_t get_vcc()
{
	uint8_t res;
	REGRI(si_vbat,res);
	return res+34;
}

static uint8_t oldboost=0;
void update_boost()
{
	uint8_t k,t;
	t=get_vcc();
	for(k=5;k>2;k--)
		if(t*k<=220)
			break;
	if(oldboost!=k)
	{
		st7558_set_boost(k-2);
		oldboost=k;
	}
}


#define N_AVG 40

uint32_t avgbuf[N_AVG];
uint8_t avgp=0;
uint32_t tavg=0;

void main(void)
{
	uint8_t mod_sel=0;
	uint32_t last_cal1=2000000l,last_cal2=2000000l;
	uint8_t repl_failed=0;
	validpkts=0;
	LED1_ON();
	PORTD|=(1<<3)|(1<<7);
	//PORTC|=(1<<3);
	init_spi();
	init_timer1();
	init_timer2();
    sei();
	st7558_init();
	st7558_setpos(0,0);
	
	memset(rssib,0,sizeof(rssib));
	memset(rssif,0,sizeof(rssif));
	
	si4432_swreset();
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
	si4432_setupgpio(si_gpio0txstate,si_gpio1rxstate,si_gpio2rxdat,0x58);
	//setup_modem(50000,50,180,0,1);
//	uint8_t selbr=eeprom_read_byte(0);
	nextmode=mod_sel=5;
	set_modem_conf(5);
	si4432_tune_base(433000000);
	si4432_hop(1,180);
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
	REGW(si_inten1,si_ipksent);
	REGW(si_inten2,si_ipreaval);
//	REGW(si_inten2,0);
	REGR(si_interrupt1,int1);
	REGR(si_interrupt2,int2);
	REGW(si_txpower,0x1f);
	REGW(si_GPIO2,si_gpio2rxstate);
	TIMSK|=(0<<TOIE1)|(0<<OCIE1A)|(1<<TICIE1)|(0<<OCIE1B);
//	TIMSK|=(1<<TICIE1);
//	REGR(0x72,pkt.fill);
	LED1_OFF();
	
	set_sleep_mode(SLEEP_MODE_IDLE);
	GIFR=0xff;
	GICR|=(1<<INT1);
	REGW(si_pklen,sizeof(ipkt));
//	REGW(si_rxafthr,sizeof(ipkt)-1);
	
    for(;;){                /* main event loop */
//		LED0_OFF();
		if(flags&FL_OC2)
		{
			cli();flags&=~FL_OC2;sei();
		}
		if(flags&FL_OVF2)
		{
			cli();flags&=~FL_OVF2;sei();
			last_cal2=last_cal1;
			last_cal1=cal2.d-cal1.d;
			if(last_cal1>8000000l/4)
				OSCCAL--;
			if(last_cal1<8000000l/4)
				OSCCAL++;
			cal1.d=cal2.d;
			update_boost();
			uint8_t tmp=0;
			REGRI(si_mode01,tmp);
			REGWI(si_mode01,si_pllon|si_xton);
			repl_failed=(tmp&si_rxon);
			REGWI(si_mode02,si_ffclrrx|si_ffclrtx);
			REGWI(si_mode02,0);
			if(txpw>7)
				txpw=0;
			REGWI(si_fifo,0);
			REGWI(si_fifo,txpw|0x18);
			REGWI(si_txpower,txpw|0x18);
			REGWI(si_mode01,si_txon|si_pllon|si_xton);
			TIMSK|=(1<<TICIE1);
		}
		if(flags&FL_INT)
		{
			cli();flags&=~FL_INT;sei();
			rssif[prssi]=frssi/7;
			uint8_t tmp1;
			tmp1=trssi/7;
			if(rssib[prssi]<tmp1)
			{
				rssib[prssi]=tmp1;
			}
			prssi++;
			if(prssi==sizeof(rssib))
				prssi=0;
			rssif[prssi]=rssib[prssi]=0;
			cli();flags|=FL_REFR;sei();
		}
		if(nextmode!=mod_sel)
		{
			if(nextmode==0x09)
				nextmode=0x10;
			if(nextmode==0x19)
				nextmode=0x20;
			if(nextmode>=0x29)
				nextmode=0x00;
			REGWI(si_mode01,mode01_def);
			set_modem_conf(mod_sel=nextmode);
			REGW(si_mode01,mode01_def|si_rxon);
			cli();flags|=FL_REFR;sei();
		}
		if(flags&FL_REFR)
		{
			cli();flags&=~FL_REFR;sei();
			pr_rssi();
			st7558_setpos(0,0);
			st7558_prchar(VC_D);
			st7558_prchar(VC_SPACE);
			if(!repl_failed)
			{
				uint32_t xx=delta.d;
				xx*=(2000000l/32l);
				xx/=(last_cal2/32);
				avgp++;
				if(avgp==N_AVG)
					avgp=0;
				tavg+=xx;
				tavg-=avgbuf[avgp];
				avgbuf[avgp]=xx;
			}
			st7558_print(tavg/(N_AVG/10),10);
//			st7558_print(xx,10);
//			st7558_print((delta.d*4000000l)/last_cal2,10);
			st7558_setpos(0,1);
			st7558_prchar(VC_N);
			st7558_prchar(VC_SPACE);
			st7558_print(last_cal2,10);
//			st7558_print(validpkts,10);
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
			st7558_prhex(txpw,1);
			st7558_prhex(mod_sel,1);
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
		if(flags==0)
		{
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}
    };
}
