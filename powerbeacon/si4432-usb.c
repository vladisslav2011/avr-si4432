/* Name: main.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-09
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id$
 */

/*
This example should run on most AVRs with only little changes. No special
hardware resources except INT0 are used. You may have to change usbconfig.h for
different I/O pins for USB. Please note that USB D+ must be the INT0 pin, or
at least be connected to INT0 as well.
We assume that an LED is connected to port B bit 0. If you connect it to a
different port or bit, change the macros below:
*/
#include "config.h"

#include <stdint.h>

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <avr/interrupt.h>  /* for sei() */
#include <util/delay.h>     /* for _delay_ms() */

#include <avr/pgmspace.h>   /* required by usbdrv.h */
#include <avr/eeprom.h>   /* required by usbdrv.h */
#include "usbdrv/usbdrv.h"
#include "genubrr.h"
#include "requests.h"       /* The custom request numbers we use */
#include "../si443x_regs.h"

/* ------------------------------------------------------------------------- */
/* ----------------------------- USB interface ----------------------------- */
/* ------------------------------------------------------------------------- */

#define HW_VER 0

#define STATE_SPI_TX       1
#define STATE_SPI_TX_OUT   2


#if defined (__AVR_ATmega8__)
#define SPI_SCK  5
#define SPI_MISO 4
#define SPI_MOSI 3
#define SPI_SS   2
#endif

#define LED0_ON() DDRD|=(1<<4)
#define LED0_OFF() DDRD&=~(1<<4)

#define GET_nIRQ() (PINB & (1<<1))
#define SET_SDN(x) if(x)PORTD|=(1<<5);else PORTD&=~(1<<5)

#define SI_CS(x) if(x)PORTB|=(1<<PB2);else PORTB&=~(1<<PB2)
#define REGW_m(r,v) do{\
	SI_CS(0);\
	SPDR=(r|0x80);\
	while((SPSR&(1<<SPIF))==0);\
	SPDR=(v);\
	while((SPSR&(1<<SPIF))==0);\
	SI_CS(1);\
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
	SI_CS(0);\
	SPDR=(r&0x7f);\
	while((SPSR&(1<<SPIF))==0);\
	SPDR=0xff;\
	while((SPSR&(1<<SPIF))==0);\
	v=SPDR;\
	SI_CS(1);\
	\
	 }while(0)

uint8_t REGR_f(uint8_t r)
{
	uint8_t res=0;
	REGR_m(r,res);
	return res;
}

#define REGR(r,v) v=REGR_f(r)
#define REGRI(r,v) do{ cli(); v=REGR_f(r) ; sei(); }while(0)

#define BURSTW(r,buf,n) do{\
	SI_CS(0);\
	SPDR=(r|0x80);\
	while((SPSR&(1<<SPIF))==0);\
	uint8_t k;\
	for(k=0;k<n;k++)\
	{\
		SPDR=(buf[k]);\
		while((SPSR&(1<<SPIF))==0);\
	}\
	SI_CS(1);\
	\
	 }while(0)

#define BURSTR(r,buf,n) do{\
	SI_CS(0);\
	SPDR=(r&0x7f);\
	while((SPSR&(1<<SPIF))==0);\
	uint8_t k;\
	for(k=0;k<n;k++)\
	{\
		SPDR=0xff;\
		while((SPSR&(1<<SPIF))==0);\
		buf[k]=SPDR;\
	}\
	SI_CS(1);\
	\
	 }while(0)

void init_spi()
{
	DDRB=(1<<PB2)|(1<<PB3)|(1<<PB5);
	//PORTB=(1<<PB0)|(1<<PB1)|(1<<PB4);
	SPCR=(0<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR0)|(1<<SPR1);
	SPCR=(1<<SPE)|(0<<DORD)|(1<<MSTR)|(0<<CPOL)|(0<<CPHA)|(1<<SPR0)|(1<<SPR1);
	SPSR=(0<<SPI2X);
}
static uint8_t bRequest=0;
static uint16_t bLength=0;
static uint16_t wValue=0;
static uint8_t burst_cs_after=0;


uint8_t intbuf[256];
uint8_t irp;
volatile uint8_t iwp;
volatile uint16_t ilast;
volatile uint8_t imin;
volatile uint8_t imax;

uint8_t rx1buf[64];
uint8_t tx1buf[64];
uint8_t rx2buf[64];
uint8_t rx1buf_len;
uint8_t rx1buf_p;
uint8_t tx1buf_len;
uint8_t tx1buf_p;
uint8_t rx2buf_len;
uint8_t rx2buf_p;
uint8_t rxtxrx_state;
uint8_t rxtxrx_rssi;

volatile uint8_t resp;

uint8_t int1,int2;

void start_responder();
void stop_responder();

usbMsgLen_t usbFunctionSetup(uchar data[8])
{
	struct usbRequest    *rq = (void *)data;
	static uint8_t replyBuffer[4];
	bRequest=rq->bRequest;
	usbMsgPtr=(void*)&replyBuffer;
	switch(bRequest)
	{
		case CUSTOM_RQ_LED:
			if(rq->bmRequestType&USBRQ_DIR_DEVICE_TO_HOST)
			{
				if(rq->wValue.bytes[0]&0x02)
				{
					if(rq->wValue.bytes[0]&0x01)
						DDRD|=(1<<4);
					else
						DDRD&=~(1<<4);
				}
				replyBuffer[0]=(DDRD&(1<<4))!=0;
				return 1;
			}else
				return 0;
		case SPI_TRANS:
			if(rq->bmRequestType&USBRQ_DIR_DEVICE_TO_HOST)
			{
				SI_CS(rq->wIndex.bytes[0]&SPI_CS_BEFORE);
				if(rq->wLength.bytes[0]>0)
				{
					SPDR=rq->wValue.bytes[0];
					while((SPSR&(1<<SPIF))==0);
					replyBuffer[0]=SPDR;
					if(rq->wLength.bytes[0]>1)
					{
						SPDR=rq->wValue.bytes[1];
						while((SPSR&(1<<SPIF))==0);
						replyBuffer[1]=SPDR;
					}
				}
				SI_CS(rq->wIndex.bytes[0]&SPI_CS_AFTER);
				return rq->wLength.bytes[0];
			}else{
				return 0;
			}
		case SPI_BURST:
			SI_CS(rq->wIndex.bytes[0]&SPI_CS_BEFORE);
			burst_cs_after=(rq->wIndex.bytes[0]&SPI_CS_AFTER);
			bLength=rq->wLength.word;
			return USB_NO_MSG;
		case CUSTOM_RXTXRX:
			bLength=rq->wLength.word;
			if(rq->bmRequestType&USBRQ_DIR_DEVICE_TO_HOST)
			{
				wValue=rq->wValue.word;
				if(wValue==4)
				{
					rxtxrx_state=0;
					return 0;
				}
				if(wValue==2)
				{
					replyBuffer[0]=rxtxrx_state;
					replyBuffer[1]=rx1buf_len;
					replyBuffer[2]=rx2buf_len;
					replyBuffer[3]=rxtxrx_rssi;
					return 4;
				}
				rx1buf_p=0;
				rx2buf_p=0;
			}else{
				tx1buf_len=bLength;
				tx1buf_p=0;
				rx1buf_len=rq->wValue.bytes[0];
				rx1buf_p=0;
				rx2buf_len=rq->wValue.bytes[1];
				rx2buf_p=0;
			}
			return USB_NO_MSG;
		case CUSTOM_INTERVALS:
			bLength=rq->wLength.word;
			if(rq->bmRequestType&USBRQ_DIR_DEVICE_TO_HOST)
			{
				if(bLength==0)
				{
//					uint8_t int1,int2;
					REGW(si_mode01,0);
					REGW(si_mode02,si_ffclrrx);
					REGW(si_mode02,si_rxmpk);
					REGW(si_preadetcon,0xf8);
					REGR(si_modcon2,int1);
					int1&=0x0f;
					REGW(si_modcon2,int1);
					TCCR1B=rq->wValue.bytes[0];
					ilast=TCNT1=0;
					TIMSK|=(1<<TICIE1);
					imin=rq->wValue.bytes[1];
					imax=rq->wIndex.bytes[0];
					irp=iwp=0;
					REGR(si_interrupt1,int1);
					REGR(si_interrupt2,int2);
					REGW(si_mode01,si_rxon);
					return 0;
				}else{
					if(rq->wValue.word&1)
					{
						REGW(si_mode01,0);
						TCCR1B=0;
						TIMSK&=~(1<<TICIE1);
					}
					if(rq->wValue.word&2)
					{
						replyBuffer[0]=iwp-irp;
						return 1;
					}
				}
				
			}else{
				//tx not implemented so far
				return 0;
			}
			return USB_NO_MSG;
		case CUSTOM_RESPONDER:
				if(rq->wValue.bytes[0])
				{
					start_responder();
				}else{
					stop_responder();
				}
			return 0;
		case CUSTOM_MV:
				ADMUX=(1<<REFS1)|(1<<REFS0)|(0<<ADLAR)|0;
				ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIF);
				ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIF);
				while(ADCSRA&(1<<ADSC));
				ADMUX=(1<<REFS1)|(1<<REFS0)|(0<<ADLAR)|1;
				(*(uint16_t*)&replyBuffer[0])=ADC;
				ADCSRA=(1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)|(1<<ADIF);
				while(ADCSRA&(1<<ADSC));
				(*(uint16_t*)&replyBuffer[2])=ADC;
			return 4;
	}
	return 0;   /* default for not implemented requests: return no data back to host */
}




/* ------------------------------------------------------------------------- */
uint8_t usbFunctionRead(uint8_t * data, uint8_t len)
{
	uint8_t k;
	if(!bLength)
		return 0;
	switch(bRequest)
	{
		case SPI_BURST:
			for(k=0;k<len;k++)
			{
				if(!bLength)
					break;
				SPDR=0;
				while((SPSR&(1<<SPIF))==0);
				data[k]=SPDR;
				bLength--;
			}
			if(!bLength)
			{
				SI_CS(burst_cs_after);
			}
			return k;
		case CUSTOM_RXTXRX:
			if(wValue==0)
			{
				for(k=0;k<len;k++)
				{
					if(!bLength)
						break;
					if(rx1buf_p>=sizeof(rx1buf))
						bLength=0;
					else{
						data[k]=rx1buf[rx1buf_p++];
						bLength--;
					}
				}
			}else{
				for(k=0;k<len;k++)
				{
					if(!bLength)
						break;
					if(rx2buf_p>=sizeof(rx2buf))
						bLength=0;
					else{
						data[k]=rx2buf[rx2buf_p++];
						bLength--;
					}
				}
			}
			return k;
		case CUSTOM_INTERVALS:
			for(k=0;(k<len) && (irp!=iwp);k++,bLength--,irp++)
			{
				if(!bLength)
					break;
				data[k]=intbuf[irp];
			}
			return k;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
uint8_t usbFunctionWrite(uint8_t * data, uint8_t len)
{
	uint8_t k;
	if(!bLength)
		return 1;
	switch(bRequest)
	{
		case SPI_BURST:
			for(k=0;k<len;k++)
			{
				SPDR=data[k];
				while((SPSR&(1<<SPIF))==0);
				data[k]=SPDR;
				bLength--;
				if(!bLength)
				{
					if(burst_cs_after)
						PORTB|=(1<<PB2);
					else
						PORTB&=~(1<<PB2);
					break;
				}
			}
			if(bLength==0)
				return 1;
		break;
		case CUSTOM_RXTXRX:
			for(k=0;k<len;k++)
			{
				tx1buf[tx1buf_p++]=data[k];
				bLength--;
				if(tx1buf_p==tx1buf_len)
					bLength=0;
				if(!bLength)
					break;
			}
			if(bLength==0)
			{
				rxtxrx_state=RXTXRX_RX1;
				return 1;
			}
		break;
	}
	return 0;
}
/* ------------------------------------------------------------------------- */
ISR(TIMER1_CAPT_vect,ISR_NOBLOCK)
{
}

volatile uint16_t t0;

ISR(TIMER1_OVF_vect,ISR_NOBLOCK)
{
	t0++;
//	DDRD^=(1<<4);
}

ISR(TIMER2_COMP_vect,ISR_NOBLOCK)
{
	PORTB^=(1<<0)|(1<<2);
}


void stop_responder()
{
	TIMSK&=~(1<<TICIE1);
	resp=0;
	REGW(si_mode01,0);
	REGW(si_mode02,si_ffclrrx|si_ffclrtx);
	REGW(si_mode02,0);
	TCCR2=0;
	DDRB&=~(1<<0);
}

void start_responder()
{
	//uint8_t int1,int2;
	#if 1
	REGW(si_inten1,0);
	REGW(si_inten2,0);
	REGW(si_pklen,2);
	REGW(si_mode02,si_ffclrrx|si_ffclrtx);
	REGW(si_mode02,0);
	REGW(si_mode01,0);
	REGR(si_interrupt1,int1);
	REGR(si_interrupt2,int2);
	#if HW_VER == 0
	REGW(si_GPIO2,si_gpio0txdat);
	#endif
	#if HW_VER == 1
	REGW(si_GPIO0,si_gpio0txdat);
	#endif
	REGW(si_fifo,0);
	REGW(si_fifo,0);
	REGW(si_mode01,si_pllon|si_xton);
	#endif
	TIFR=(1<<TICIE1)|(1<<OCIE2)|(1<<TOIE1);
	TIMSK|=(1<<OCIE2)|(1<<TOIE1);
	//TIMSK|=(1<<TOIE1);
	REGW(si_data_access_contorl,0);
	REGW(si_modcon1,si_txdtrtscale);
	//REGW(si_modcon2,si_modtyp_fsk);
	REGW(si_modcon2,si_modtyp_gfsk|si_trclk_nirq);
	REGW(si_fdl,2);
	DDRB|=(1<<0);
	TCCR2=(1<<WGM21)|(0<<WGM20)|(1<<CS22)|(0<<CS21)|(1<<CS20)|0;
	TCCR1B=(0<<CS22)|(1<<CS21)|(1<<CS20)|0;
}


uint8_t r_state=0;
uint8_t got_rssi=0;
enum R_STATES{
	R_ENTER_WATCH=0,
	R_WATCH,
	R_DET1,
	R_CW_PREPARE,
	R_CW_SEND,
	R_BEACON_PREPARE,
	R_BEACON_SEND,
};

#define len_0 0x0f
#define p_0 0x0f
//#define len_1 0x1e
#define len_1 (0x0f*3)
#define p_1 0x0f
#define pp (0x0f*3)

void add_0()
{
	intbuf[iwp++]=0x80|len_0;
	intbuf[iwp++]=p_0;
}

void add_1()
{
	intbuf[iwp++]=0x80|len_1;
	intbuf[iwp++]=p_1;
}

void add_p()
{
	intbuf[iwp++]=pp;
}

void cw_num(uint8_t in)
{
	switch(in)
	{
	case 0: 
		add_1();add_1();add_1();add_1();add_1();add_p();
	break;
	case 1: 
		add_0();add_1();add_1();add_1();add_1();add_p();
	break;
	case 2: 
		add_0();add_0();add_1();add_1();add_1();add_p();
	break;
	case 3: 
		add_0();add_0();add_0();add_1();add_1();add_p();
	break;
	case 4: 
		add_0();add_0();add_0();add_0();add_1();add_p();
	break;
	case 5: 
		add_0();add_0();add_0();add_0();add_0();add_p();
	break;
	case 6: 
		add_1();add_0();add_0();add_0();add_0();add_p();
	break;
	case 7: 
		add_1();add_1();add_0();add_0();add_0();add_p();
	break;
	case 8: 
		add_1();add_1();add_1();add_0();add_0();add_p();
	break;
	case 9: 
		add_1();add_1();add_1();add_1();add_0();add_p();
	break;
	case 0x0a: 
		add_0();add_1();add_p();
	break;
	case 0x0b: 
		add_1();add_0();add_0();add_0();add_p();
	break;
	case 0x0c: 
		add_1();add_0();add_1();add_0();add_p();
	break;
	case 0x0d: 
		add_1();add_0();add_0();add_p();
	break;
	case 0x0e: 
		add_0();add_p();
	break;
	case 0x0f: 
		add_0();add_0();add_1();add_0();add_p();
	break;
	case 0xff: 
		add_p();
		add_p();
	break;
	default:
		add_1();add_0();add_p();
		add_1();add_0();add_p();
	}
}







void radioPoll()
{
	uint8_t int2;
	static uint8_t det;
	static uint16_t pt1=0;
	static uint16_t cnt=0;
	uint16_t t1;
	
	cli();
	t1=t0;
	sei();
	switch(r_state)
	{
	case R_WATCH://idle
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			if(int2 & si_ipreaval)
			{
				REGR(si_rssi,got_rssi);
				r_state=R_DET1;
				TCNT1=0;
				t0=0;
				det=0;
				LED0_ON();
			}else{
				if(t1>50)
					r_state=R_ENTER_WATCH;
			}
		return;
	case R_DET1:
			REGR(si_interrupt1,int1);
			REGR(si_interrupt2,int2);
			if(t1>4)
				det=1;
			if(!(int2 & si_ipreaval))
			{
				LED0_OFF();
				if(det==1)
					r_state=R_BEACON_PREPARE;
				if(det==0)
					r_state=R_CW_PREPARE;
			}
		return;
	case R_ENTER_WATCH:
		REGW(si_mode01,1);
		si4432_setup_modem(2000,2,4,0,1,si_modtyp_gfsk);
		REGW(si_data_access_contorl,0);
		REGW(si_inten1,0);
		REGW(si_inten2,0);
		REGW(si_mode01,7);
		REGW(si_preadetcon,(8<<si_preath_shift)|0);
		t0=0;
		r_state=R_WATCH;
		return;
	case R_CW_PREPARE:
		LED0_ON();
		iwp=irp=0;
/*		cw_num(got_rssi>>4);
		cw_num(got_rssi&0x0f);*/
		
		cnt++;
		
		cw_num(got_rssi/100);
		cw_num((got_rssi/10)%10);
		cw_num(got_rssi%10);
		
		cw_num(0xff);
		
		cw_num((cnt/100)%10);
		cw_num((cnt/10)%10);
		cw_num(cnt%10);
		
		
/*		cw_num((det/10)%10);
		cw_num(det%10);*/
		
		start_responder();
		//TCCR1B=(0<<CS22)|(1<<CS21)|(0<<CS20)|0;
		TCNT1=0;
		t0=0;
		
		
		
				TIMSK&=~(1<<OCIE2);
		REGW(0x6d,0x1f);
		REGW(si_mode01,si_txon|si_pllon|si_xton);
		OCR2=0x40;
		r_state=R_CW_SEND;
		return;
	case R_CW_SEND:
		if(t1!=pt1)
		{
			pt1=t1;
			if(t1<3)
				return;
			if(iwp==irp)
			{
				stop_responder();
				r_state=R_ENTER_WATCH;
				LED0_OFF();
				return;
			}
			uint8_t to_int=intbuf[irp++];
			if(to_int&0x80)
			{
				TIMSK|=(1<<OCIE2);
			}else{
				TIMSK&=~(1<<OCIE2);
			}
			to_int&=0x7f;
/*			if(to_int&0x10)
				TCNT1=0;
			else
				TCNT1=0x7fff;*/
			TCNT1=0xffff-(to_int<<10);
/*			}else{
				REGW(0x6d,0x18);
				REGW(si_mode01,si_pllon|si_xton);
			}*/
		}
		return;
	case R_BEACON_PREPARE:
		t0=0;
		start_responder();
		r_state=R_BEACON_SEND;
		return;
	case R_BEACON_SEND:
		if(t1!=pt1)
		{
			pt1=t1;
			if(t1>0x20)
			{
				stop_responder();
				r_state=R_ENTER_WATCH;
				return;
			}
/*			if(t1&0x010)
			{*/
				uint8_t ta=((t1>>1)&0x07);
				OCR2=0x80 + (ta<<1);
				REGW(0x6d,0x18|ta);
				if(t1&0x0001)
				{
					TIMSK|=(1<<OCIE2);
				}else{
					TIMSK&=~(1<<OCIE2);
				}
				REGW(si_mode01,si_txon|si_pllon|si_xton);
/*			}else{
				REGW(0x6d,0x18);
				REGW(si_mode01,si_pllon|si_xton);
			}*/
		}
		return;
	default://all done
		return;
	}
}




void init_usart()
{
	UBRRL=GENUBRR(115200)&0xff;
	UBRRH=(GENUBRR(115200)>>8)&0xff;
	UCSRA=GENU2X(115200);
	UCSRC=(1<<URSEL)|(0<<UPM0)|(0<<UPM1)|(0<<USBS)|(1<<UCSZ1)|(1<<UCSZ0);
	UCSRB=(1<<RXEN)|(1<<TXEN)|(0<<UCSZ2);
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




void main(void)
{
	uchar   i;
	rxtxrx_state=0;
	SET_SDN(1);
	DDRD|=(1<<4)|(1<<5);
/*	PORTC=(1<<1)|(1<<2);
	DDRC=(1<<0)|(1<<1);*/
	PORTC=0;
	DDRC=0;
    usbDeviceDisconnect();  /* enforce re-enumeration, do this while interrupts are disabled! */
    usbInit();
    init_usart();
    i = 0;
	SET_SDN(0);
    while(--i){             /* fake USB disconnect for > 250 ms */
        _delay_ms(1);
    }
	init_spi();
	si4432_swreset();
	#if HW_VER == 0
	si4432_setupgpio(si_gpio0txstate,si_gpio1rxstate,si_gpio2rxdat,0x00);
	#endif
	#if HW_VER == 1
	si4432_setupgpio(si_gpio0rxdat,si_gpio1txstate,si_gpio2rxstate,0x00);
	#endif
	set_modem_conf(1);
	si4432_tune_base(433000350);
	si4432_hop(1,170);
	REGW(si_headcon1,0);
	REGW(si_headcon2,si_fixpklen|si_syncword3210);
//	REGW(si_headcon2,0);
	REGW(si_prealenl,16);
	REGW(si_sync3,0x0f);
	REGW(si_sync2,0x34);
	REGW(si_sync1,0x21);
	REGW(si_sync0,0xff);
	REGW(si_data_access_contorl,si_enpacrx|si_enpactx|si_encrc|si_crc16);
	//start_responder();
 	set_sleep_mode(SLEEP_MODE_IDLE);
    usbDeviceConnect();
    sei();
	DDRD&=~(1<<4);
    for(;;){                /* main event loop */
/*		if((rxtxrx_state&0x7f)<=0)
		{
			sleep_enable();
			sleep_cpu();
			sleep_disable();
		}*/
		//LED0_ON();
		usbPoll();
		
/*		if(iwp!=irp)
			if(UCSRA&(1<<UDRE))
			{
				UDR=intbuf[irp++];
			}*/
/*		if(usbInterruptIsReady())
		{
			if(iwp!=irp)
				usbSetInterrupt(&intbuf[irp++],1);
		}*/
		//LED0_OFF();
		radioPoll();
		//state machine here
/*		if(resp==2)
		{
			uint8_t tmp,int1,int2;
			REGR(si_ezmac_stat,tmp);
			if(tmp&si_pksent)
			{
				REGW(si_mode02,si_ffclrrx|si_ffclrtx);
				REGW(si_mode02,0);
				REGR(si_interrupt1,int1);
				REGR(si_interrupt2,int2);
				REGW(si_fifo,0);
				REGW(si_fifo,0);
				REGW(si_mode01,si_rxon|si_pllon|si_xton);
				resp=1;
			}
		}*/
    };
}

/* ------------------------------------------------------------------------- */
