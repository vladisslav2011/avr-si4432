#! /bin/bash
k="`./test txpk 1001800000000001 trxpk 8 300 2>/dev/null|grep 901`"
p1=0x`echo ${k#* }|sed -e 's/^..\(..\).*/\1/'`;
p2=`echo ${k#* }|sed -e 's/^....\(..\).*/\1/'`;
p3=`echo ${k#* }|sed -e 's/^......\(..\).*/\1/'`;
s=0x`echo ${k#* }|sed -e 's/^.\{8\}\(..\)\(..\)\(..\)\(..\)/\4\3\2\1/'`;
echo ${k% *}:$p2/$p3 pwr=$[p1-0x18] seq=$[s]
