


/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/
/*-----------------------------------------------------------------------------------*/

#ifndef HOOKS_INIT

H_NEW(usbchunk, "	\"--usbchunk <size>\"		set maximum usb transfer size\n"
				"	\"--usbchunk get\"		get current maximum usb transfer size setting\n"

)
{
//	char buffer[1];
	if(strcasecmp(argv[*argp], "--usbchunk") == 0)
	{
		++ *argp;
		if(*argp == argc)
			return 2;
		if(strcasecmp(argv[*argp], "get") != 0)
		{
			unsigned newsize=atoi(argv[*argp]);
			if(newsize > 0)
				if(newsize <= 254 )
					usbchunk=newsize;
		}
		printf("Usb chunk size set to %u \n",usbchunk);
		++ *argp;
		return 0;
	}
	return 2;
}



#else

H_MERGE(usbchunk)

#endif

