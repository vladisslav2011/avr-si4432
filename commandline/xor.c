#define _FILE_OFFSET_BITS 64
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>


#define MIN(a,b) ((a>b)?b:a)



int main(int argc, char ** argv)
{
	if(argc<3)
	{
		fprintf(stderr,"At least two files required\n");
		return 1;
	};
	int fda=open(argv[1],O_RDONLY);
	if(fda<0)
	{
		fprintf(stderr,"Failed to open file 1\n");
		return 2;
	}
	int fdb=open(argv[2],O_RDONLY);
	if(fdb<0)
	{
		fprintf(stderr,"Failed to open file 2\n");
		return 3;
	}
	int fdx=fileno(stdin);
	if(argc==4)
	{
		fdx=open(argv[3],O_RDWR|O_CREAT,0664);
		if(fdx<0)
		{
			fprintf(stderr,"Failed to open output file \n");
			return 3;
		}
	}
	#define BUF_SIZE 1024*1024
	unsigned char * bufa = malloc(BUF_SIZE);
	if(bufa == NULL)
	{
		fprintf(stderr,"Failed to allocate buffer \n");
		return 10;
	};
	unsigned char * bufb = malloc(BUF_SIZE);
	if(bufb == NULL)
	{
		fprintf(stderr,"Failed to allocate buffer \n");
		return 10;
	};
	off_t pos=0;
	while(1)
	{
		unsigned gota=read(fda,bufa,BUF_SIZE);
		unsigned gotb=read(fdb,bufb,BUF_SIZE);
		unsigned l=MIN(gota,gotb);
		if(l==0)
		{
			if(gota!=gotb)
			{
				fprintf(stderr,"Done, size differs \n");
				close(fda);
				close(fdb);
				close(fdx);
				free(bufa);
				free(bufb);
				return 20;
			}else{
				fprintf(stderr,"Done \n");
				close(fda);
				close(fdb);
				close(fdx);
				free(bufa);
				free(bufb);
				return 0;
			}
		}
		unsigned char *pa=bufa;
		unsigned char *pb=bufb;
		while(l--)
			*(pa++) ^= *(pb++);
		if(write(fdx,bufa,MIN(gota,gotb))<MIN(gota,gotb))
		{
			fprintf(stderr,"Write failed, exiting \n");
			close(fda);
			close(fdb);
			close(fdx);
			free(bufa);
			free(bufb);
			return 20;
			
		};
		if(MIN(gota,gotb)<BUF_SIZE)
		{
			if(gota!=gotb)
			{
				fprintf(stderr,"Done, size differs \n");
				close(fda);
				close(fdb);
				close(fdx);
				free(bufa);
				free(bufb);
				return 20;
			}else{
				fprintf(stderr,"Done \n");
				close(fda);
				close(fdb);
				close(fdx);
				free(bufa);
				free(bufb);
				return 0;
			}
			
		}
		
	}
	
	
	
};


