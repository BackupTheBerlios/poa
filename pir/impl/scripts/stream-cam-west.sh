#!/bin/sh
HOST=129.69.210.63
SIZE=640x480
AUTO=day
BR=0
BACKL=0
SAT=0
SHARP=0
NSUPP=high

while true
do
   sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-`date +%s`.jpg
done
