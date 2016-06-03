#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define I_SHORT ((0x5c+0x89)/2)
#define I_LONG  ((0x89+0xb5)/2)


int main()
{
	uint8_t buf;
	uint8_t rxed[1024*4];
	int rlen=0;
	int p1=0,p2=0;
	int byte=0;
	int bp=0;
	int k=0;
	while(1)
	{
		if(fread(&buf,1,1,stdin)<=0)
			break;
		if(p1==3)
		{
			if(p2==1)
			{
				if(buf>I_LONG)
				{
					p1=1;
					p2=0;
					byte=0;
					bp=0;
					int t;
					if(rlen>3)
					{
						for(t=0;t<rlen;t++)
							printf("%02x",rxed[t]);
						printf(",%d\n",k);
						fflush(stdout);
					}
					rlen=0;
					k=0;
				}else{
					byte<<=1;
					if(buf<I_SHORT)
						byte|=0x01;
					bp++;
					if(bp==8)
					{
						rxed[rlen++]=byte;
						byte=0;
						bp=0;
					}
					k++;
				}
			}else{
				if(buf<I_SHORT)
					p2++;
				else{
					p2=0;
					p1=0;
					bp=0;
					rlen=0;
				}
			}
		}else{
			if(buf>I_LONG)
			{
				p1++;
			}else{
				p1=0;
				p2=0;
				bp=0;
				rlen=0;
			}
		}
		
	}
	return 0;
}
