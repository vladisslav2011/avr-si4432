#! /bin/bash
kk="$1"
#tx=`printf '1001%02x00%08x' 24 $kk`
tx=`printf '10000000%08x' $kk`
k="`./test txpk $tx trxpk 8 500 2>/dev/null|grep 901`"
p1=0x`echo ${k#* }|sed -e 's/^..\(..\).*/\1/'`;
p2=`echo ${k#* }|sed -e 's/^....\(..\).*/\1/'`;
p3=`echo ${k#* }|sed -e 's/^......\(..\).*/\1/'`;
s=0x`echo ${k#* }|sed -e 's/^.\{8\}\(..\)\(..\)\(..\)\(..\)/\4\3\2\1/'`;
echo ${k% *}:$p2/$p3 pwr=$[p1-0x18] seq=$[s]
