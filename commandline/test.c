/* Name: set-led.c
 * Project: custom-class, a basic USB example
 * Author: Christian Starkjohann
 * Creation Date: 2008-04-10
 * Tabsize: 4
 * Copyright: (c) 2008 by OBJECTIVE DEVELOPMENT Software GmbH
 * License: GNU GPL v2 (see License.txt), GNU GPL v3 or proprietary (CommercialLicense.txt)
 * This Revision: $Id$
 */

/*
General Description:
This is the host-side driver for the custom-class example device. It searches
the USB for the LEDControl device and sends the requests understood by this
device.
This program must be linked with libusb on Unix and libusb-win32 on Windows.
See http://libusb.sourceforge.net/ or http://libusb-win32.sourceforge.net/
respectively.
*/
#define _GNU_SOURCE
#define F_CPU 16000000
#define _FILE_OFFSET_BITS 64


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <termios.h>
#include <errno.h>
#include <usb.h>        /* this is libusb */
#include "opendevice.h" /* common code moved to separate module */

#include "../firmware/requests.h"   /* custom request numbers */
#include "../firmware/usbconfig.h"  /* device's VID/PID and names */

usb_dev_handle      *handle = NULL;
static struct termios old;


struct s_hook;
struct s_hook
{
	struct s_hook * next;
	char * help;
	int (*process) (int *argp, int argc, char ** argv) ;
};

static struct s_hook * hooks = NULL;




static unsigned char usbchunk=254;
unsigned long long restartaddr=0;
static unsigned char spi_mode=0;











int filewrite(char *fn,void* data,int size)
{
	int fd=open(fn,O_RDWR|O_CREAT|O_TRUNC,0644);
	if(fd<0)
	{
		fprintf(stderr,"[filewrite]Error opening the file \"%s\"\n",fn);
		return -1;
	}
	int written=write(fd,data,size);
	if(written<0)
	{
		fprintf(stderr,"[filewrite]Error writing the file \"%s\"\n",fn);
	}
	if(close(fd)<0)
	{
		fprintf(stderr,"[filewrite]Error closing the file \"%s\"\n",fn);
	
	};
	return written;
}

int fileread(char *fn,void* data,int size)
{
	int fd=open(fn,O_RDONLY);
	if(fd<0)
	{
		fprintf(stderr,"[fileread]Error opening the file \"%s\"\n",fn);
		return -1;
	}
	int readb=read(fd,data,size);
	if(readb<0)
	{
		fprintf(stderr,"[fileread]Error reading the file \"%s\"\n",fn);
	}
	if(close(fd)<0)
	{
		fprintf(stderr,"[fileread]Error closing the file \"%s\"\n",fn);
	
	};
	return readb;
}








#define H_CONCAT(a,b) a##b
#define mconcat2(a,b) a##b
#define mconcat3(a,b,c) a##b##c
#define mconcat4(a,b,c,d) a##b##c##d
#define mconcat5(a,b,c,d,e) a##b##c##d##e

#define H_STRUCT(x,hlp) struct H_CONCAT(s_hook hook_function_,x) =\
{\
	next:NULL,\
	help:hlp,\
	process:&function_t1\
};

#define H_NEW(x,hlp) int H_CONCAT(h_function_,x) (int *argp, int argc, char ** argv);\
struct H_CONCAT(s_hook hook_function_,x) =\
{\
	next:NULL,\
	help:hlp,\
	process:& H_CONCAT(h_function_,x)\
};\
int H_CONCAT(h_function_,x) (int *argp, int argc, char ** argv)

#define H_MERGE(x)\
*c=& H_CONCAT(hook_function_,x) ;\
c=& (*c)->next;


#include "functions.h"




void usage(char *name)
{
	struct s_hook * c=hooks;
	fprintf(stderr, "usage:\n");
	while(c!=NULL)
	{
		fprintf(stderr,"%s",c->help);
		c=c->next;
	}
}

void hooks_init()
{
	struct s_hook ** c = & hooks;
#define HOOKS_INIT
#include "functions.h"
#undef HOOKS_INIT
}


void tc_restore()
{
	tcsetattr(fileno(stdin),TCSAFLUSH,&old);
}



int main(int argc, char **argv)
{
	struct termios term;
	fprintf(stderr,"%d\n",fcntl(fileno(stdout),F_GETFL,NULL));
	tcgetattr(fileno(stdin),&term);
	tcgetattr(fileno(stdin),&old);
	
	term.c_lflag &= ~ICANON;
	term.c_lflag &= ~ECHO;
	atexit(&tc_restore);
	tcsetattr(fileno(stdin),TCSANOW,&term);
//	fcntl(fileno(stdin),F_SETFL,fcntl(fileno(stdin),F_GETFL,NULL)|O_NONBLOCK);
	fprintf(stderr,"%d\n",fcntl(fileno(stdout),F_GETFL,NULL));
	
	
	
	const unsigned char rawVid[2] = {USB_CFG_VENDOR_ID}, rawPid[2] = {USB_CFG_DEVICE_ID};
	char                vendor[] = {USB_CFG_VENDOR_NAME, 0}, product[] = {USB_CFG_DEVICE_NAME, 0};
	int                 vid, pid;
	
		hooks_init();
		usb_init();
		if(argc < 2){   /* we need at least one argument */
			usage(argv[0]);
			exit(1);
		}
		/* compute VID/PID from usbconfig.h so that there is a central source of information */
		vid = rawVid[1] * 256 + rawVid[0];
		pid = rawPid[1] * 256 + rawPid[0];
		/* The following function is in opendevice.c: */
		if(usbOpenDevice(&handle, vid, vendor, pid, product, NULL, NULL, NULL) != 0){
			fprintf(stderr, "Could not find USB device \"%s\" with vid=0x%x pid=0x%x\n", product, vid, pid);
			exit(1);
		}
#if 0
    int retries = 1, usbConfiguration = 1, usbInterface = 0;
    if(usb_set_configuration(handle, usbConfiguration) && showWarnings){
        fprintf(stderr, "Warning: could not set configuration: %s\n", usb_strerror());
    }
    /* now try to claim the interface and detach the kernel HID driver on
     * Linux and other operating systems which support the call. */
    while((len = usb_claim_interface(handle, usbInterface)) != 0 && retries-- > 0){
#ifdef LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP
        if(usb_detach_kernel_driver_np(handle, 0) < 0 && showWarnings){
            fprintf(stderr, "Warning: could not detach kernel driver: %s\n", usb_strerror());
        }
#endif
    }
#endif
	struct s_hook * c=hooks;
	int argp=1;
	int res=0;
	int matched=0;
	while(c!=NULL)
	{
		res=c->process(&argp,argc,argv);
		if(res!=2)
			matched=1;
		if(res&~2)
			break;
		if(argp >= argc)
			break;
		if(res!=2)
			c=hooks;
		else
			c=c->next;
	}
	if(!matched)
		usage(argv[0]);
	usb_close(handle);
	exit(res&~2);
	return 0;
}
