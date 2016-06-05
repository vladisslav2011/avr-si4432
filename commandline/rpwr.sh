#! /bin/bash
pwr="$1";
pwr=$[pwr&0x7f];
printf "%02x\n" $pwr
./test txpk `printf "02%02x0000%08x" $pwr 0`