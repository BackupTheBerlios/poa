#!/bin/sh
HOST=129.69.210.63
SIZE=640x480
AUTO=day
BACKL=0
BR=0
SAT=0
SHARP=0
NSUPP=high

sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-norm-`date +%s`.jpg
sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-norm-`date +%s`.jpg


BR=5
SAT=5
sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-high-`date +%s`.jpg
sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-high-`date +%s`.jpg

BR=-5
SAT=-5
sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-low-`date +%s`.jpg
sh fetch.sh $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP west/west-low-`date +%s`.jpg
