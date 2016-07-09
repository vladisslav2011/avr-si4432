/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#define READ_PARAM(ptype,pvar,fname) do{\
		++ *argp;\
		if(*argp == argc)\
		{\
			fprintf(stderr,fname ": " #pvar " reading failed. More parameters required\n");\
			return 1;\
		}\
		if(sscanf(argv[*argp],ptype,&pvar)!=1)\
		{\
			fprintf(stderr,fname ": invalid parameter " #pvar "\"%s\"\n",argv[*argp]);\
			return 1;\
		}\
		}while(0)

#define READ_STR(pvar,fname) do{\
		++ *argp;\
		if(*argp == argc)\
		{\
			fprintf(stderr,fname ": " #pvar " reading failed. More parameters required\n");\
			return 1;\
		}\
		pvar=argv[*argp];\
		}while(0)




#ifndef HOOKS_INIT

H_NEW(in,"	\"led 0/1/get\"			perform!\n")
{
	if(strcasecmp(argv[*argp], "led") == 0)
	{
		char * arg=argv[*argp];
		READ_PARAM("%s",*arg,"led");
		++ *argp;
		int wValue=0;
		if(strcasecmp(arg,"get")!=0)
			wValue|=2;
		if(strcasecmp(arg,"1")==0)
			wValue|=1;
		char buffer=0;
		int cnt = usb_control_msg(
			handle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			CUSTOM_RQ_LED,//int request
			wValue,//int value
			0,//int index
			&buffer,//char *bytes
			1,//int size
			5000);//int timeout
		if(cnt < 1)
		{
			if(cnt < 0)
			{
				fprintf(stderr, "USB error: %s\n", usb_strerror());
				return 1;
			}//else
			//	fprintf(stderr, "only %d bytes received.\r", cnt);
		}
		printf("%d\n",buffer);
		return 0;
	}
	return 2;
}


#else

H_MERGE(in)

#endif


/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

#define si_get_iffbw_H2(x) \
	((x<=2.6)?81:((x<=2.8)?82:((x<=3.1)?83:((x<=3.2)?84:((x<=3.7)?85:((x<=4.9)?65:((x<=5.4)?\
	66:((x<=5.9)?67:((x<=6.1)?68:((x<=7.2)?69:((x<=9.5)?49:((x<=10.6)?50:((x<=11.5)?51:\
	((x<=12.1)?52:((x<=14.2)?53:((x<=16.2)?54:((x<=17.5)?58:((x<=19.4)?59:((x<=21.4)?60:\
	((x<=23.9)?61:((x<=25.7)?62:((x<=28.2)?37:((x<=32.2)?38:((x<=34.7)?39:((x<=38.6)?43:\
	((x<=42.7)?44:((x<=47.7)?45:((x<=51.2)?46:((x<=56.2)?21:((x<=64.1)?22:((x<=69.2)?23:\
	((x<=75.2)?1:((x<=83.2)?2:((x<=90.0)?3:((x<=95.3)?4:((x<=112.1)?5:((x<=127.9)?6:((x<=137.9)?\
	7:((x<=142.8)?148:((x<=167.8)?149:((x<=181.1)?153:((x<=191.5)?150:((x<=208.4)?154:\
	((x<=225.1)?129:((x<=248.8)?130:((x<=269.3)?131:((x<=284.9)?132:((x<=335.5)?136:((x<=361.8)?\
	137:((x<=420.2)?138:((x<=468.4)?139:((x<=518.8)?140:((x<=577.0)?141:((x<=620.7)?142:142)\
	)))))))))))))))))))))))))))))))))))))))))))))))))))))

	
#define si_get_iffbw_H210(x) \
	((x<=2.6)?81:((x<=2.8)?82:((x<=3.1)?83:((x<=3.2)?84:((x<=3.7)?85:((x<=4.9)?65:((x<=5.4)?66:\
	((x<=5.9)?67:((x<=6.1)?68:((x<=7.2)?69:((x<=9.5)?49:((x<=10.6)?50:((x<=11.5)?51:((x<=12.1)?\
	52:((x<=14.2)?53:((x<=16.2)?54:((x<=17.5)?55:((x<=18.9)?33:((x<=21.6)?34:((x<=22.7)?35:\
	((x<=24.0)?36:((x<=28.2)?42:((x<=31.9)?31:((x<=34.7)?39:((x<=38.6)?43:((x<=42.7)?44:((x<=47.7)?\
	45:((x<=51.2)?46:((x<=56.2)?21:((x<=64.1)?22:((x<=69.2)?23:((x<=75.2)?1:((x<=83.2)?2:\
	((x<=90.0)?3:((x<=95.3)?4:((x<=112.1)?5:((x<=127.9)?6:((x<=137.9)?7:((x<=138.7)?10:((x<=154.2)?\
	11:((x<=168.0)?152:((x<=181.1)?153:((x<=208.4)?154:((x<=232.0)?155:((x<=256.0)?156:\
	((x<=269.3)?131:((x<=284.9)?132:((x<=335.5)?136:((x<=361.8)?137:((x<=420.2)?138:\
	((x<=468.4)?139:((x<=518.8)?140:((x<=577.0)?141:((x<=620.7)?142:142))))))))))))))))))))))))))))))))))))))))))))))))))))))



	
#define si_get_iffbw_H10(x) \
	((x<=2.6)?81:((x<=2.8)?82:((x<=3.1)?83:((x<=3.2)?84:((x<=3.7)?85:((x<=4.9)?65:((x<=5.4)?66:\
	((x<=5.9)?67:((x<=6.1)?68:((x<=7.2)?69:((x<=9.5)?49:((x<=10.6)?50:((x<=11.5)?51:((x<=12.1)?\
	52:((x<=14.2)?53:((x<=16.2)?54:((x<=17.5)?58:((x<=19.4)?59:((x<=21.4)?60:((x<=23.9)?61:\
	((x<=25.7)?62:((x<=28.2)?37:((x<=32.2)?38:((x<=34.7)?39:((x<=38.6)?43:((x<=42.7)?44:((x<=47.7)?\
	45:((x<=51.2)?46:((x<=56.2)?21:((x<=64.1)?22:((x<=69.4)?26:((x<=77.1)?27:((x<=85.1)?\
	28:((x<=95.3)?29:((x<=102.2)?30:((x<=115.6)?171:((x<=127.7)?172:((x<=142.8)?173:((x<=153.3)?\
	174:((x<=168.0)?152:((x<=181.1)?153:((x<=208.4)?154:((x<=232.0)?155:((x<=256.0)?156:\
	((x<=269.3)?131:((x<=284.9)?132:((x<=335.5)?136:((x<=361.8)?137:((x<=420.2)?138:((x<=468.4)?\
	139:((x<=518.8)?140:((x<=577.0)?141:((x<=620.7)?142:((x<=620.7)?142:142))))))))))))))))))))))))))))))))))))))))))))))))))))))

	
#define si_get_iffbw(x) si_get_iffbw_H2(x)

#define si_make_iffbw_lut_f_H10 {\
{2.6,81},\
{2.8,82},\
{3.1,83},\
{3.2,84},\
{3.7,85},\
{4.9,65},\
{5.4,66},\
{5.9,67},\
{6.1,68},\
{7.2,69},\
{9.5,49},\
{10.6,50},\
{11.5,51},\
{12.1,52},\
{14.2,53},\
{16.2,54},\
{17.5,58},\
{19.4,59},\
{21.4,60},\
{23.9,61},\
{25.7,62},\
{28.2,37},\
{32.2,38},\
{34.7,39},\
{38.6,43},\
{42.7,44},\
{47.7,45},\
{51.2,46},\
{56.2,21},\
{64.1,22},\
{69.4,26},\
{77.1,27},\
{85.1,28},\
{95.3,29},\
{102.2,30},\
{115.6,171},\
{127.7,172},\
{142.8,173},\
{153.3,174},\
{168.0,152},\
{181.1,153},\
{208.4,154},\
{232.0,155},\
{256.0,156},\
{269.3,131},\
{284.9,132},\
{335.5,136},\
{361.8,137},\
{420.2,138},\
{468.4,139},\
{518.8,140},\
{577.0,141},\
{620.7,142},\
}


#define si_make_iffbw_lut_f_H2_10 {\
{2.6,81},\
{2.8,82},\
{3.1,83},\
{3.2,84},\
{3.7,85},\
{4.9,65},\
{5.4,66},\
{5.9,67},\
{6.1,68},\
{7.2,69},\
{9.5,49},\
{10.6,50},\
{11.5,51},\
{12.1,52},\
{14.2,53},\
{16.2,54},\
{17.5,55},\
{18.9,33},\
{21.6,34},\
{22.7,35},\
{24.0,36},\
{28.2,42},\
{31.9,31},\
{34.7,39},\
{38.6,43},\
{42.7,44},\
{47.7,45},\
{51.2,46},\
{56.2,21},\
{64.1,22},\
{69.2,23},\
{75.2,1},\
{83.2,2},\
{90.0,3},\
{95.3,4},\
{112.1,5},\
{127.9,6},\
{137.9,7},\
{138.7,10},\
{154.2,11},\
{168.0,152},\
{181.1,153},\
{208.4,154},\
{232.0,155},\
{256.0,156},\
{269.3,131},\
{284.9,132},\
{335.5,136},\
{361.8,137},\
{420.2,138},\
{468.4,139},\
{518.8,140},\
{577.0,141},\
}

#define si_make_iffbw_lut_f_H2 {\
{2.6,81},\
{2.8,82},\
{3.1,83},\
{3.2,84},\
{3.7,85},\
{4.9,65},\
{5.4,66},\
{5.9,67},\
{6.1,68},\
{7.2,69},\
{9.5,49},\
{10.6,50},\
{11.5,51},\
{12.1,52},\
{14.2,53},\
{16.2,54},\
{17.5,58},\
{19.4,59},\
{21.4,60},\
{23.9,61},\
{25.7,62},\
{28.2,37},\
{32.2,38},\
{34.7,39},\
{38.6,43},\
{42.7,44},\
{47.7,45},\
{51.2,46},\
{56.2,21},\
{64.1,22},\
{69.2,23},\
{75.2,1},\
{83.2,2},\
{90.0,3},\
{95.3,4},\
{112.1,5},\
{127.9,6},\
{137.9,7},\
{142.8,148},\
{167.8,149},\
{181.1,153},\
{191.5,150},\
{208.4,154},\
{225.1,129},\
{248.8,130},\
{269.3,131},\
{284.9,132},\
{335.5,136},\
{361.8,137},\
{420.2,138},\
{468.4,139},\
{518.8,140},\
{577.0,141},\
}

#define si_ook_ndec(br) \
((br<=1000)?5:((br<=2000)?5:((br<=3000)?4:((br<=8000)?4:((br<=16000)?3:((br<=32000)?2:((br<=50000)?1:0)))))))


#define si_ook_filset(bw) \
((bw<=75.2)?0x01:((bw<=83.2)?0x02:((bw<=90)?0x03:((bw<=95.3)?0x04:((bw<=112.1)?0x05:((bw<=127.9)?\
0x06:((bw<=137.9)?0x07:((bw<=191.5)?0x80:((bw<=225.1)?0x81:((bw<=248.8)?0x82:((bw<=269.3)?0x83:\
((bw<=284.9)?0x84:((bw<=335.5)?0x88:((bw<=361.8)?0x89:((bw<=420.2)?0x8A:((bw<=468.4)?0x8B:\
((bw<=518.8)?0x8C:((bw<=577)?0x8D:0x8E))))))))))))))))))

#define si_ook_iffbw(Rb,bw) (si_ook_filset(bw)|(si_ook_ndec(Rb)<<4))




uint8_t get_iffbw_lut(double in,double H)
{
	struct IFFBW_LUT {double f;uint8_t iffbw;};
	struct IFFBW_LUT lut_H2[]  =si_make_iffbw_lut_f_H2;
	struct IFFBW_LUT lut_H210[]=si_make_iffbw_lut_f_H2_10;
	struct IFFBW_LUT lut_H10[] =si_make_iffbw_lut_f_H10;
	struct IFFBW_LUT * lut=(struct IFFBW_LUT *)(H<=2)?&lut_H2[0]:((H<=10)?&lut_H210[0]:&lut_H10[0]);
	int k=0;
	for(;k<sizeof(lut_H2)/sizeof(lut[0]);k++)
		if(in<=lut[k].f)
			return lut[k].iffbw;
	return 142;
}

int spi_transfer(uint8_t * buf,int len, int cs_before,int cs_after)
{
	if(len>2)
		return -1;
	int cnt = usb_control_msg(
		handle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		SPI_TRANS,//int request
		(len>0)?(buf[0]|((len>1)?(buf[1]<<8):0)):0,//int value
		(cs_before?SPI_CS_BEFORE:0)|(cs_after?SPI_CS_AFTER:0)|(0<<8),//int index
		(char*)buf,//char *bytes
		len,//int size
		5000);//int timeout
	return cnt;
}

int set_reg(uint8_t reg, uint8_t data)
{
	uint8_t buf[2];
	buf[0]=reg |0x80;
	buf[1]=data;
	return spi_transfer(buf,2,0,1);
}

int get_reg(uint8_t reg)
{
	uint8_t buf[2];
	buf[0]=reg &0x7f;
	buf[1]=0;
	if(spi_transfer(buf,2,0,1)==2)
		return buf[1];
	else
		return -1;
}

int burst_read(uint8_t * buf, int len, int cs_before,int cs_after)
{
	return usb_control_msg(
		handle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
		SPI_BURST,//int request
		0,//int value
		(cs_before?SPI_CS_BEFORE:0)|(cs_after?SPI_CS_AFTER:0),//int index
		(char*)buf,//char *bytes
		len,//int size
		5000);//int timeout
}

int burst_write(uint8_t * buf, int len, int cs_before,int cs_after)
{
	return usb_control_msg(
		handle,
		USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
		SPI_BURST,//int request
		0,//int value
		(cs_before?SPI_CS_BEFORE:0)|(cs_after?SPI_CS_AFTER:0),//int index
		(char*)buf,//char *bytes
		len,//int size
		5000);//int timeout
}


H_NEW(sidump,"	\"sidump\"			dump all si443x regs\n")
{
	if(strcasecmp(argv[*argp], "sidump") == 0)
	{
		++ *argp;
		int k;
		uint8_t buf[2];
		for(k=0;k<128;k++)
		{
			buf[0]=k&0x7f;
			buf[1]=0;
			int cnt;
			if((cnt=spi_transfer(buf,2,0,1))==2)
				printf("[%02x]=%02x\n",k,buf[1]);
			else
				printf("err=%d\n",cnt);
		}
		return 0;
	}
	return 2;
}


#else

H_MERGE(sidump)

#endif



/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(set,"	\"set reg data\"			set si443x register\n")
{
	if(strcasecmp(argv[*argp], "set") == 0)
	{
		int reg=-1;
		int value=-1;
		READ_PARAM("%i",reg,"set");
		READ_PARAM("%i",value,"set");
		++ *argp;
		uint8_t buf[2];
		buf[0]=(reg&0x7f)|0x80;
		buf[1]=value;
		int cnt;
		if((cnt=spi_transfer(buf,2,0,1))==2)
			printf("set [%02x]=%02x, result: %02x %02x\n",reg,value,buf[0],buf[1]);
		else
			printf("err=%d\n",cnt);
		return 0;
	}
	return 2;
}


#else

H_MERGE(set)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(get,"	\"get reg\"			get si443x register\n")
{
	if(strcasecmp(argv[*argp], "get") == 0)
	{
		int reg=-1;
		READ_PARAM("%i",reg,"get");
		++ *argp;
		uint8_t buf[2];
		buf[0]=reg&0x7f;
		buf[1]=0;
		int cnt;
		if((cnt=spi_transfer(buf,2,0,1))==2)
			printf("[%02x]=%02x\n",reg,buf[1]);
		else
			printf("err=%d\n",cnt);
		return 0;
	}
	return 2;
}


#else

H_MERGE(get)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(tune,"	\"tune freq\"			tune si443x pll\n")
{
	if(strcasecmp(argv[*argp], "tune") == 0)
	{
		double fr=0;
		READ_PARAM("%lf",fr,"tune");
		++ *argp;
		int hbsel=(fr>=480000000)?1:0;
		int fb=fr/10000000.0/(hbsel+1)-24.0;
		int fc=(fr/10000000.0/(hbsel+1)-24.0-fb)*64000.0;
		int fc1=(fr/10000000.0/(hbsel+1)-24.0-((int)(fr/10000000.0/(hbsel+1)-24.0)))*64000.0;
		printf("hbsel=%d\nfb=%02x\nfc=%02x\nfc1=%02x\n",hbsel,fb,fc,fc1);
		set_reg(0x75,(1<<6)|(hbsel<<5)|(fb&0x1f));
		set_reg(0x76,(fc>>8)&0xff);
		set_reg(0x77,fc&0xff);
		
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(tune)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(hop,"	\"hop sz freq\"			offset tune si443x pll\n")
{
	if(strcasecmp(argv[*argp], "hop") == 0)
	{
		int sz=0;
		int fr=0;
		READ_PARAM("%i",sz,"hop");
		READ_PARAM("%i",fr,"hop");
		++ *argp;
		printf("hop=%d kHz\n",sz*fr*10);
		set_reg(0x7A,sz);
		set_reg(0x79,fr);
		
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(hop)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(Fd,"	\"Fd deviation\"			set tx deviation\n")
{
	if(strcasecmp(argv[*argp], "Fd") == 0)
	{
		double Fd=0;
		READ_PARAM("%lf",Fd,"Fd");
		++ *argp;
		int fd=Fd*1000/625;
		printf("fd=%03x\n",fd);
		
		set_reg(0x71,(get_reg(0x71)&~(1<<2))|((fd>>(8-2))&(1<<2)));
		set_reg(0x72,fd&0xff);
		
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(Fd)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(txdr,"	\"txdr rate\"			set tx data rate\n")
{
	if(strcasecmp(argv[*argp], "txdr") == 0)
	{
		double dr=0;
		READ_PARAM("%lf",dr,"txdr");
		++ *argp;
		int txdr=dr*(1<<16)/1000000;
		int slow=0;
		if(dr<30000.0)
		{
			slow=1;
			txdr=dr*(1<<21)/1000000;
		}
		set_reg(0x6f,txdr&0xff);
		set_reg(0x6e,(txdr>>8)&0xff);
		set_reg(0x70,(get_reg(0x70)&~(1<<5))|(slow<<5));
		
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(txdr)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(setupgpio,"	\"setupgpio\"			setup si443x gpio\n")
{
	if(strcasecmp(argv[*argp], "setupgpio") == 0)
	{
		++ *argp;
		set_reg(0x0b,0x12);//TX state
		set_reg(0x0c,0x15);//rx state
		set_reg(0x0d,0x14);//rx data
		
		set_reg(0x09,0x37);//tune crystal to exactly 30000000Hz
		
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(setupgpio)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT


H_NEW(setbw,"	\"setbw bw\"			setfilter badwidth\n")
{
	if(strcasecmp(argv[*argp], "setbw") == 0)
	{
		double bw=0;
		READ_PARAM("%lf",bw,"setbw");
		++ *argp;
		
		int iffbw=si_get_iffbw_H2(bw);
		printf("iffbw=%02x\n",iffbw);
		set_reg(0x1c,iffbw);
		int dwn3_bypass=(iffbw&0x80)!=0;
		int ndec_exp=(iffbw>>4)&0x7;
		int enmanch=0;
		int Rb=40;
		int Fd=100;
		int rxosr=500*8*(1+2*dwn3_bypass)/((1<<ndec_exp)*Rb*(1+enmanch));
		int ncoff=Rb*(1+enmanch)*(1<<(20+ndec_exp))/(500*(1+2*dwn3_bypass));
		int crgain=2+(1<<16)*(1+enmanch)*Rb/rxosr/Fd;
		printf("rxosr=%d\nncoff=%d\ncrgain=%d\n",rxosr,ncoff,crgain);
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(setbw)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(setup_modem,"	\"setup-modem bitrate deviation bandwidth manchester mod inversion\"			setup modem\n"
"		bitrate, bps\n"
"		bandwidth, kHz\n"
"		manchester, xxxx, 0=off,1=on\n"
"		            ^^^^\n"
"		 manchester /|||\n"
"		 data inv    /||\n"
"		 pream inv    /|\n"
"		 white en      /\n"
"		mod=none|OOK|FSK|GFSK\n"
"		inversion, 0=off,1=on\n"
"\n"


)
{
	if(strcasecmp(argv[*argp], "setup-modem") == 0)
	{
		double br=0;
		double deviation=0;
		double bandwidth=0;
		char * enmanchs=NULL;
		int inversion=0;
		char * mod=NULL;
		READ_PARAM("%lf",br,"setup-modem");
		READ_PARAM("%lf",deviation,"setup-modem");
		READ_PARAM("%lf",bandwidth,"setup-modem");
		READ_STR(enmanchs,"setup-modem");
		READ_STR(mod,"setup-modem");
		READ_PARAM("%i",inversion,"setup-modem");
		++ *argp;
		if(strlen(enmanchs)!=4)
		{
			fprintf(stderr,"setup-modem: manchester: invalid value\n");
			return 2;
		}
		int enmanch=0;
		int manchinv=0;
		int manchpreaminv=0;
		int datawhite=0;
		if(enmanchs[0]!='0')enmanch=1;
		if(enmanchs[1]!='0')manchinv=1;
		if(enmanchs[2]!='0')manchpreaminv=1;
		if(enmanchs[3]!='0')datawhite=1;
		
		printf("br=%lf deviation=%lf bandwidth=%lf\n",br,deviation,bandwidth);
		int fd=deviation*1000/625;
		printf("fd=%d 0x%04x\n",fd,fd);
		
		uint8_t reg70=get_reg(0x70)&~0x0f;
		reg70|=0x10;
		if(manchpreaminv)
			reg70|=0x08;
		if(manchinv)
			reg70|=0x04;
		if(enmanch)
			reg70|=0x02;
		if(datawhite)
			reg70|=0x01;
			
		int modi=0;
		if(strcasecmp(mod,"OOK") == 0)
			modi=1;
		if(strcasecmp(mod,"FSK") == 0)
			modi=2;
		if(strcasecmp(mod,"GFSK") == 0)
			modi=3;
		if(inversion)
			modi|=0x08;
		double H=2.0*deviation*1000.0/br/(1.0+enmanch);
		printf("H=%lf\n",H);
		double BWmod=br/1000.0*(1.0+enmanch)+2.0*deviation;
		printf("BWmod=%lf\n",BWmod);
		
		int txdr=br*(1ll<<16)/1000000.0;
		int slow=0;
		if(br<30000.0)
		{
			slow=1;
			txdr=br*(1ll<<21)/1000000.0;
		}
		printf("txdr=%d 0x%04x\n",txdr,txdr);

//		int iffbw=si_get_iffbw(bandwidth);
		int iffbw=(modi==1)?si_ook_iffbw(br,bandwidth):get_iffbw_lut(bandwidth,H);
		printf("iffbw=0x%02x\n",iffbw);
		int dwn3_bypass=(iffbw&0x80)!=0;
		int ndec_exp=(iffbw>>4)&0x7;
		double Rb=br/1000.0;
		int rxosr=500.0*8.0*(1.0+2.0*dwn3_bypass)/((1<<ndec_exp)*Rb*(1+enmanch));
//		int ncoff=Rb*(1+enmanch)*(1<<(20+ndec_exp))/(500.0*(1+2*dwn3_bypass));
		int ncoff=Rb*(1<<(20+ndec_exp))/(500.0*(1+2*dwn3_bypass));
		int crgain=2+(1<<15)*(1+enmanch)*Rb/rxosr/deviation;
		int ookcnt=1500000/(br*(1+enmanch));
		int hbsel=0;
		int afc_bw_lim=1000.0*bandwidth/2.0/(hbsel+1)/156.25/4.0;
		if(afc_bw_lim>80)
			afc_bw_lim=80;
		printf("rxosr=%d 0x%04x\n",rxosr,rxosr);
		printf("ncoff=%d 0x%04x\n",ncoff,ncoff);
		printf("crgain=%d 0x%04x\n",crgain,crgain);
		printf("ookcnt=%d 0x%04x\n",ookcnt,ookcnt);
		if(crgain>0x07ff)
		{
			crgain>>=1;
			crgain|=0x0800;
		}
		if(ncoff>=(1<<20))
			ncoff=(1<<20)-1;
		printf("ncoff*=%d 0x%04x\n",ncoff,ncoff);
		printf("crgain*=%d 0x%04x\n",crgain,crgain);
		
		uint8_t reg71=get_reg(0x71);
		uint8_t new71=(reg71&~0x0f)|((fd>>(8-2))&(1<<2))|modi;
		printf("reg71 0x%02x => 0x%02x\n",reg71,new71);
		set_reg(0x71,new71);
		reg70&=~(1<<5);
		reg70|=(slow<<5);
		set_reg(0x70,reg70);
		printf("[70]=%02x\n",reg70);
		printf("[71]=%02x\n",new71);
		set_reg(0x72,fd&0xff);
		
		set_reg(0x6f,txdr&0xff);
		set_reg(0x6e,(txdr>>8)&0xff);
		
		set_reg(0x1c,iffbw);
		
		set_reg(0x20,rxosr&0xff);
		set_reg(0x21,((rxosr>>3)&0xe0)|((ncoff>>16)&0x0f));
		set_reg(0x22,(ncoff>>8)&0xff);
		set_reg(0x23,ncoff&0xff);
		set_reg(0x24,(crgain>>8)&0x0f);
		set_reg(0x25,crgain&0xff);
		set_reg(0x2c,0x18|((ookcnt>>8)&0xff)|(enmanch?0:0x20));
		set_reg(0x2d,ookcnt&0xff);
		set_reg(0x2a,afc_bw_lim);
		//set_reg(0x1d,0x80|0x40|0x04);
		set_reg(0x1d,0x40|0x04);
		
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(setup_modem)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(setup_packet,"	\"setup-packet prealen sync synclen crc fixlen\"   \n")
{
	if(strcasecmp(argv[*argp], "setup-packet") == 0)
	{
		uint32_t prealen=0;
		uint32_t sync=0;
		char * syncc=NULL;
		int synclen=0;
		char * crca=NULL;
		int fixlen=0;
		READ_PARAM("%i",prealen,"setup-packet");
		READ_STR(syncc,"setup-packet");
		READ_PARAM("%i",synclen,"setup-packet");
		READ_STR(crca,"setup-packet");
		READ_PARAM("%i",fixlen,"setup-packet");
		++ *argp;
		
		int k;
		int ln=strlen(syncc);
		char *p=syncc;
		for(k=0;k<ln;k++)
		{
			uint8_t m=0;
			if((p[k]>='0')&&(p[k]<='9'))
				m=p[k]-'0';
			if((p[k]>='A')&&(p[k]<='F'))
				m=p[k]-'A'+10;
			if((p[k]>='a')&&(p[k]<='f'))
				m=p[k]-'a'+10;
			sync <<=4;
			sync|=m;
		}
		printf("prealen=%d,sync=%08x,synclen=%d,crca=%s,fixlen=%d \n",prealen,sync,synclen,crca,fixlen);
		
		
		set_reg(0x36,(sync&0xff000000)>>24);
		set_reg(0x37,(sync&0x00ff0000)>>16);
		set_reg(0x38,(sync&0x0000ff00)>>8);
		set_reg(0x39,(sync&0x000000ff));
		if(synclen<1)
			synclen=1;
		if(synclen>4)
			synclen=4;
		--synclen;
		set_reg(0x32,0);
		set_reg(0x33,((fixlen>=0)?0x08:0)|(synclen<<1)|((prealen&0x010)>>8));
		set_reg(0x34,prealen&0xff);
		if(fixlen>0)
			set_reg(0x3e,fixlen);
		int crci=-1;
		if(strcasecmp(crca,"CCITT") == 0)
			crci=0;
		if(strcasecmp(crca,"CRC16") == 0)
			crci=1;
		if(strcasecmp(crca,"IEC16") == 0)
			crci=2;
		if(strcasecmp(crca,"Biacheva") == 0)
			crci=3;

		set_reg(0x30,0x88|((crci>=0)?(0x04|crci):0));
		set_reg(0x71,(get_reg(0x71)&~0x30)|0x20);
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(setup_packet)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(txpk,"	\"txpk 010203a0a1...\n")
{
	if(strcasecmp(argv[*argp], "txpk") == 0)
	{
		char * p=NULL;
		READ_STR(p,"txpk");
		++ *argp;
		
		int ln=strlen(p);
		if(ln&1)
		{
			fprintf(stderr,"txpk: invalid hex str\n");
			return 1;
		}
		
		set_reg(0x3e,ln/2);
		
		uint8_t * buf=calloc(1,ln/2);
		int k;
		for(k=0;k<ln;k++)
		{
			uint8_t m=0;
			if((p[k]>='0')&&(p[k]<='9'))
				m=p[k]-'0';
			if((p[k]>='A')&&(p[k]<='F'))
				m=p[k]-'A'+10;
			if((p[k]>='a')&&(p[k]<='f'))
				m=p[k]-'a'+10;
			if(k&1)
				buf[k/2]|=m;
			else
				buf[k/2]=m<<4;
		}
		uint8_t bb=0xff;
		spi_transfer(&bb,1,0,0);
		burst_write(buf,ln/2,0,1);
		set_reg(0x05,0x40);
		set_reg(0x06,0x00);
		printf("status=%02x %02x\n",get_reg(0x03),get_reg(0x04));
		set_reg(0x07,9);
		while(get_reg(0x07)&0x08);
		printf("done\n");
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(txpk)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(rxpk,"	\"rxpk len\n")
{
	if(strcasecmp(argv[*argp], "rxpk") == 0)
	{
		int ln;
		READ_PARAM("%i",ln,"rxpk");
		++ *argp;
		
		if(ln>0)
		{
			set_reg(0x3e,ln);
			set_reg(0x33,get_reg(0x33)|0x80);
		}else{
			set_reg(0x3e,255);
			set_reg(0x33,get_reg(0x33)&~0x80);
		}
		
		int k;
		set_reg(0x08,0x03);
		set_reg(0x08,0x00);
		set_reg(0x05,0x00);
		set_reg(0x06,0x02);
		printf("status=%d %d\n",get_reg(0x03),get_reg(0x04));
		set_reg(0x07,5);
		uint8_t irssi=get_reg(0x26);
		while(get_reg(0x07)& 0x04);
		if(ln<0)
			ln=get_reg(0x4b);
		
		printf("rssi=%02x/%02x ",get_reg(0x26),irssi);
		uint8_t * buf=malloc(ln);
		uint8_t rn=0x7f;
		spi_transfer(&rn,1,0,0);
		burst_read(buf,ln,0,1);
		for(k=0;k<ln;k++)
		{
			printf("%02x",buf[k]);
		}
		free(buf);
		printf("\n");
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(rxpk)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(trxpk,"	\"trxpk len to - rx with timeout (bytes,ms)\n")
{
	if(strcasecmp(argv[*argp], "trxpk") == 0)
	{
		int ln;
		uint64_t ms;
		READ_PARAM("%i",ln,"trxpk");
		READ_PARAM("%llu",ms,"trxpk");
		++ *argp;
		
		if(ln>0)
		{
			set_reg(0x3e,ln);
			set_reg(0x33,get_reg(0x33)|0x80);
		}else{
			set_reg(0x3e,255);
			set_reg(0x33,get_reg(0x33)&~0x80);
		}
		
		int k;
		set_reg(0x08,0x03);
		set_reg(0x08,0x00);
		set_reg(0x05,0x00);
		set_reg(0x06,0x02);
		printf("status=%d %d\n",get_reg(0x03),get_reg(0x04));
		set_reg(0x07,5);
		uint8_t irssi=get_reg(0x26);
		struct timeval ts;//start
		struct timeval tn;//now
		gettimeofday(&ts,NULL);
		while(get_reg(0x07)& 0x04)
		{
			if(ln<0)
				ln=get_reg(0x4b);
			gettimeofday(&tn,NULL);
			if(tn.tv_sec*1000+tn.tv_usec/1000-ts.tv_sec*1000-ts.tv_usec/1000 > ms)
			{
				return 0;
			}
		}
		printf("rssi=%02x/%02x ",get_reg(0x26),irssi);
		uint8_t * buf=malloc(ln);
		uint8_t rn=0x7f;
		spi_transfer(&rn,1,0,0);
		burst_read(buf,ln,0,1);
		for(k=0;k<ln;k++)
		{
			printf("%02x",buf[k]);
		}
		free(buf);
		printf("\n");
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(trxpk)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(rxloop,"	\"rxloop len\n")
{
	if(strcasecmp(argv[*argp], "rxloop") == 0)
	{
		int ln;
		READ_PARAM("%i",ln,"rxloop");
		
		++ *argp;
		
		if(ln>0)
		{
			set_reg(0x3e,ln);
			set_reg(0x33,get_reg(0x33)|0x80);
		}else{
			set_reg(0x3e,255);
			set_reg(0x33,get_reg(0x33)&~0x80);
		}
		
		int k;
		set_reg(0x08,0x03);
		set_reg(0x08,0x00);
		set_reg(0x05,0x02);
		set_reg(0x06,0x00);
		printf("status=%d %d\n",get_reg(0x03),get_reg(0x04));
		while(1)
		{
			set_reg(0x07,5);
			int tt=0;
			while(get_reg(0x07)& 0x04)
			{
				++tt;
			};
			if(ln<0)
				ln=get_reg(0x4b);
			
			for(k=0;k<ln;k++)
			{
				printf("%02x",get_reg(0x7f));
			}
			printf("\n");
		}
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(rxloop)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(rxloopr,"	\"rxloopr len - rx loop with rssi\n")
{
	if(strcasecmp(argv[*argp], "rxloopr") == 0)
	{
		int ln;
		READ_PARAM("%i",ln,"rxloopr");
		
		++ *argp;
		
		if(ln>0)
		{
			set_reg(0x3e,ln);
			set_reg(0x33,get_reg(0x33)|0x80);
		}else{
			set_reg(0x3e,255);
			set_reg(0x33,get_reg(0x33)&~0x80);
		}
		
		int k;
		set_reg(0x08,0x03);
		set_reg(0x08,0x00);
		set_reg(0x05,0x02);
		set_reg(0x06,0x00);
		#define RSSIBUFLEN 256
		int rssibuf[RSSIBUFLEN];
		int rssibufp=0;
		int rssibufwrap=0;
		while(1)
		{
			set_reg(0x07,5);
			printf("status=%02x %02x\n",get_reg(0x03),get_reg(0x04));
			int tt=0;
			for(k=0;k<RSSIBUFLEN;k++)
				rssibuf[k]=-1;
			rssibufp=0;
			rssibufwrap=0;
			while(get_reg(0x07)& 0x04)
			{
				if((tt&0x1f)==0)
				{
					printf("\rrssi=");
					fflush(stdout);
				}
				uint8_t crssi=get_reg(0x26);
				rssibuf[rssibufp++]=crssi;
				if(rssibufp>=RSSIBUFLEN)
				{
					rssibufp=0;
					rssibufwrap=1;
				}
				printf("%02x ",crssi);
				++tt;
			};
			if(ln<0)
				ln=get_reg(0x4b);
			
			int maxrssi=-1;
			int avgrssi=0;
			int to=rssibufwrap?RSSIBUFLEN:rssibufp;
			for(k=0;k<to;k++)
			{
				if(rssibuf[k]>maxrssi)
					maxrssi=rssibuf[k];
				avgrssi+=rssibuf[k];
			}
			if(k==0)
				k=1;
			int thr=avgrssi/k;
			avgrssi=0;
			int nn=0;
			for(k=0;k<to;k++)
				if(rssibuf[k]<thr)
				{
					avgrssi+=rssibuf[k];
					nn++;
				};
			if(nn==0)
				nn=1;
			avgrssi/=nn;
			
			
			printf("\n");
			printf("status=%02x %02x rssi=%02x avg=%02x snr=%d\n",get_reg(0x03),get_reg(0x04),maxrssi,avgrssi,maxrssi-avgrssi);
			for(k=0;k<ln;k++)
			{
				printf("%02x",get_reg(0x7f));
			}
			printf("\n");
		}
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(rxloopr)

#endif


/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(rxtxrx,"	\"rxtxrx len <byte><byte>... len - rx,tx,rx  sequence\n")
{
	if(strcasecmp(argv[*argp], "rxtxrx") == 0)
	{
		int ln1;
		int ln2;
		char * p;
		READ_PARAM("%i",ln1,"rxtxrx");
		READ_STR(p,"rxtxrx");
		READ_PARAM("%i",ln2,"rxtxrx");
		++ *argp;
		
		if(ln1>0)
		{
			set_reg(0x3e,ln1);
			set_reg(0x33,get_reg(0x33)|0x80);
		}else{
			fprintf(stderr,"rxtxrx: variable length is not supported...\n");
			return 2;
			set_reg(0x3e,255);
			set_reg(0x33,get_reg(0x33)&~0x80);
		}
		if(ln2<0)
		{
			fprintf(stderr,"rxtxrx: variable length is not supported...\n");
			return 2;
			set_reg(0x3e,255);
			set_reg(0x33,get_reg(0x33)&~0x80);
		}
		
		uint8_t txlen=strlen(p);
		if(txlen&1)
		{
			fprintf(stderr,"rxtxrx: invalid hex str\n");
			return 1;
		}
		uint8_t * txbuf=calloc(1,txlen/2);
		int k;
		for(k=0;k<txlen;k++)
		{
			uint8_t m=0;
			if((p[k]>='0')&&(p[k]<='9'))
				m=p[k]-'0';
			if((p[k]>='A')&&(p[k]<='F'))
				m=p[k]-'A'+10;
			if((p[k]>='a')&&(p[k]<='f'))
				m=p[k]-'a'+10;
			if(k&1)
				txbuf[k/2]|=m;
			else
				txbuf[k/2]=m<<4;
		}
		
		set_reg(0x08,0x03);
		set_reg(0x08,0x00);
		printf("status=%02x %02x\n",get_reg(0x03),get_reg(0x04));
		int cnt = usb_control_msg(
			handle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_OUT,
			CUSTOM_RXTXRX,//int request
			(ln1&0x3f)|((ln2&0x3f)<<8),//int value
			0,//int index
			(char *)txbuf,//char *bytes
			txlen/2,//int size
			5000);//int timeout
		printf("usb=%d\n",cnt);
		free(txbuf);
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(rxtxrx)

#endif



/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(rxtxrxpoll,"	\"rxtxrxpoll - rx,tx,rx  sequence result poll\n")
{
	if(strcasecmp(argv[*argp], "rxtxrxpoll") == 0)
	{
		++ *argp;
		uint8_t rx1len=0;
		uint8_t rx2len=0;
		uint8_t * buf=calloc(1,256);
		int done=0;
		int cnt;
		
		while(!done)
		{
			cnt = usb_control_msg(
				handle,
				USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				CUSTOM_RXTXRX,//int request
				2,//int value
				0,//int index
				(char *)buf,//char *bytes
				4,//int size
				5000);//int timeout
//			printf("usb=%d",cnt);
			if(cnt<=0)
			{
				printf(" err...\n");
				free(buf);
				return 0;
			}
			printf(" status=%d rx1len=%d rx2len=%d rssi=%02x\r",buf[0],buf[1],buf[2],buf[3]);
			if(buf[0]==RXTXRX_IDLE)
			{
				printf("\nIdle, nothing to do\n");
				free(buf);
				return 0;
			}
			if(buf[0]==RXTXRX_DONE)
			{
				printf("\n");
				rx1len=buf[1];
				rx2len=buf[2];
				cnt = usb_control_msg(
					handle,
					USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
					CUSTOM_RXTXRX,//int request
					0,//int value
					0,//int index
					(char *)buf,//char *bytes
					rx1len,//int size
					5000);//int timeout
				if(cnt<0)
				{
					printf("usb=%d err...\n",cnt);
					free(buf);
					return 0;
				}
				//debug...
				if(rx1len>8)
				{
					printf("%04d-%02d-%02d %02d:%02d:%02d [%s] ",
						*(uint16_t*)&buf[5],
						*(uint8_t*)&buf[4],
						*(uint8_t*)&buf[3],
						*(uint8_t*)&buf[2],
						*(uint8_t*)&buf[1],
						*(uint8_t*)&buf[0],
						((*(uint8_t*)&buf[7])>0)?"R":"S"
						);
				}
				
				
				for(cnt=0;cnt<rx1len;cnt++)
					printf("%02x",buf[cnt]);
				printf("\n");
				cnt = usb_control_msg(
					handle,
					USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
					CUSTOM_RXTXRX,//int request
					1,//int value
					0,//int index
					(char *)buf,//char *bytes
					rx2len,//int size
					5000);//int timeout
				if(cnt<0)
				{
					printf("usb=%d err...\n",cnt);
					free(buf);
					return 0;
				}
				if(rx2len>8)
				{
					printf("%04d-%02d-%02d %02d:%02d:%02d [%s] ",
						*(uint16_t*)&buf[5],
						*(uint8_t*)&buf[4],
						*(uint8_t*)&buf[3],
						*(uint8_t*)&buf[2],
						*(uint8_t*)&buf[1],
						*(uint8_t*)&buf[0],
						((*(uint8_t*)&buf[7])>0)?"R":"S"
						);
				}
				for(cnt=0;cnt<rx2len;cnt++)
					printf("%02x",buf[cnt]);
				printf("\n");
				cnt = usb_control_msg(
					handle,
					USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
					CUSTOM_RXTXRX,//int request
					4,//int value
					0,//int index
					NULL,//char *bytes
					0,//int size
					5000);//int timeout
				return 0;
			}
		}
		return 0;
	}
	return 2;
}


#else

H_MERGE(rxtxrxpoll)

#endif


/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(istart,"	\"istart div min max\n")
{
	if(strcasecmp(argv[*argp], "istart") == 0)
	{
		int cnt;
//		int mode;
		int idiv;
		int imin;
		int imax;
//		char * p;
		uint8_t buf[256];
		uint8_t tccr1b=0;
		READ_PARAM("%i",idiv,"istart");
//		READ_STR(p,"istart");
		READ_PARAM("%i",imin,"istart");
		READ_PARAM("%i",imax,"istart");
		++ *argp;
		switch(idiv)
		{
		case 1:tccr1b=1;break;
		case 8:tccr1b=2;break;
		case 64:tccr1b=3;break;
		case 256:tccr1b=4;break;
		case 1024:tccr1b=5;break;
		default:
			fprintf(stderr,"istart:Invalid div value: %d \n",idiv);
			return 1;
		}
		tccr1b|=0x80;
//		tccr1b|=0x40;
		cnt = usb_control_msg(
			handle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			CUSTOM_INTERVALS,//int request
			tccr1b|((imin&0xff)<<8),//int value
			imax&0xff,//int index
			NULL,//char *bytes
			0,//int size
			5000);//int timeout
		int kk=0;
		int k=0;
/*		while(1)
		{
			kk++;
			cnt = usb_interrupt_read(
				handle,
				0x81,
				(char *) buf,
				16,
				100);
			if(cnt==1)
			{
				printf("%02x ",buf[0]);
				k++;
				if((k&15)==0)
					printf(" \n");
			}
		}*/
		return 0;
		while(1)
		{
			kk++;
			cnt = usb_control_msg(
				handle,
				USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
				CUSTOM_INTERVALS,//int request
				2,//int value
				0,//int index
				(char*) buf,//char *bytes
				1,//int size
				5000);//int timeout
			fprintf(stderr,"                                   len=%d cnt=%d %d \r",buf[0],cnt,kk);
			uint8_t ll=buf[0];
			if(ll>0)
			{
				cnt = usb_control_msg(
					handle,
					USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
					CUSTOM_INTERVALS,//int request
					0,//int value
					0,//int index
					(char*) buf,//char *bytes
					ll,//int size
					5000);//int timeout
				for(k=0;k<cnt;k++)
				{
					printf("%02x ",buf[k]);
					if((k&15)==0)
						printf(" \n");
				}
				printf(" \n");
				fflush(stdout);
			}else
				usleep(500000);
			
		}
		
		return 0;
	}
	return 2;
}


#else

H_MERGE(istart)

#endif

/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(istop,"	\"istop\n")
{
	uint8_t buf[8];
	int cnt;
	if(strcasecmp(argv[*argp], "istop") == 0)
	{
		++ *argp;
		cnt = usb_control_msg(
			handle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			CUSTOM_INTERVALS,//int request
			3,//int value
			0,//int index
			(char *)buf,//char *bytes
			1,//int size
			5000);//int timeout
		printf("buffer=%d\n",buf[0]);
		return 0;
	}
	return 2;
}


#else

H_MERGE(istop)

#endif

#ifndef HOOKS_INIT

H_NEW(resp,"	\"resp 0/1\"			set auto responder\n")
{
	if(strcasecmp(argv[*argp], "resp") == 0)
	{
		char * arg=argv[*argp];
		READ_PARAM("%s",*arg,"resp");
		++ *argp;
		int wValue=0;
		if(strcasecmp(arg,"1")==0)
			wValue=1;
		char buffer=0;
		int cnt = usb_control_msg(
			handle,
			USB_TYPE_VENDOR | USB_RECIP_DEVICE | USB_ENDPOINT_IN,
			CUSTOM_RESPONDER,//int request
			wValue,//int value
			0,//int index
			&buffer,//char *bytes
			1,//int size
			5000);//int timeout
		if(cnt < 1)
		{
			if(cnt < 0)
			{
				fprintf(stderr, "USB error: %s\n", usb_strerror());
				return 1;
			}//else
			//	fprintf(stderr, "only %d bytes received.\r", cnt);
		}
		printf("%d\n",buffer);
		return 0;
	}
	return 2;
}


#else

H_MERGE(resp)

#endif







#undef READ_STR
#undef READ_PARAM
