#! /bin/bash
hop=$1
echo hop=$hop
./test txpk `printf '03%02x%02x00%08x' 1 $hop 0`  hop 1 $hop