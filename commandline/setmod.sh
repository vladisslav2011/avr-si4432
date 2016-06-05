#! /bin/bash
if [ "$2" = 1 ] ; then
	./test setupgpio set 0x09 0x58 setup-modem 1200 1 6 0000 GFSK 0
fi

m=0x$1
m=$[ m+0]
echo m=$m
./test  tune 433000000 hop 1 92 setup-packet 8 0f3421ff 4 CRC16 8 set 0x6d 0x18 \
 txpk `printf "%02x%02x%04x%08x" 1 $m 0 0` txpk `printf "%02x%02x%04x%08x" 0 0 0 0`


case $m  in
  0 ) ./test setupgpio set 0x09 0x58 setup-modem   1200   1   4 0000 GFSK 0 ;;
  1 ) ./test setupgpio set 0x09 0x58 setup-modem   2400   2   8 0000 GFSK 0 ;;
  2 ) ./test setupgpio set 0x09 0x58 setup-modem   4800   5  15 0000 GFSK 0 ;;
  3 ) ./test setupgpio set 0x09 0x58 setup-modem   9600   5  22 0000 GFSK 0 ;;
  4 ) ./test setupgpio set 0x09 0x58 setup-modem  19200  15  55 0000 GFSK 0 ;;
  5 ) ./test setupgpio set 0x09 0x58 setup-modem  38400  50 142 0000 GFSK 0 ;;
  6 ) ./test setupgpio set 0x09 0x58 setup-modem  50000  50 160 0000 GFSK 0 ;;
  7 ) ./test setupgpio set 0x09 0x58 setup-modem 100000 100 300 0000 GFSK 0 ;;
  8 ) ./test setupgpio set 0x09 0x58 setup-modem 200000 200 600 0000 GFSK 0 ;;
  
 16 ) ./test setupgpio set 0x09 0x58 setup-modem   1200 100 202 0000 GFSK 0 ;;
 17 ) ./test setupgpio set 0x09 0x58 setup-modem   2400 100 204 0000 GFSK 0 ;;
 18 ) ./test setupgpio set 0x09 0x58 setup-modem   4800 100 208 0000 GFSK 0 ;;
 19 ) ./test setupgpio set 0x09 0x58 setup-modem   9600 100 215 0000 GFSK 0 ;;
 20 ) ./test setupgpio set 0x09 0x58 setup-modem  19200 100 230 0000 GFSK 0 ;;
 21 ) ./test setupgpio set 0x09 0x58 setup-modem  38400 100 250 0000 GFSK 0 ;;
 22 ) ./test setupgpio set 0x09 0x58 setup-modem  50000 100 270 0000 GFSK 0 ;;
 23 ) ./test setupgpio set 0x09 0x58 setup-modem 100000 100 300 0000 GFSK 0 ;;
 24 ) ./test setupgpio set 0x09 0x58 setup-modem 200000 100 600 0000 GFSK 0 ;;
 
 32 ) ./test setupgpio set 0x09 0x58 setup-modem   1200  75 200 0000  OOK 0 ;;
 33 ) ./test setupgpio set 0x09 0x58 setup-modem   2400  75  75 0000  OOK 0 ;;
 34 ) ./test setupgpio set 0x09 0x58 setup-modem   4800  75  75 0000  OOK 0 ;;
 35 ) ./test setupgpio set 0x09 0x58 setup-modem   9600  75  75 0000  OOK 0 ;;
 36 ) ./test setupgpio set 0x09 0x58 setup-modem  19200  75  75 0000  OOK 0 ;;
 37 ) ./test setupgpio set 0x09 0x58 setup-modem  38400  75 130 0000  OOK 0 ;;
 38 ) ./test setupgpio set 0x09 0x58 setup-modem  50000 110 160 0000  OOK 0 ;;
 39 ) ./test setupgpio set 0x09 0x58 setup-modem 100000 200 300 0000  OOK 0 ;;
 40 ) ./test setupgpio set 0x09 0x58 setup-modem 200000 400 600 0000  OOK 0 ;;
  
 
 
esac;
./test  tune 433000000 hop 1 92 setup-packet 8 0f3421ff 4 CRC16 8 set 0x6d 0x18
