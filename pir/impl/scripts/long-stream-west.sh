#!/bin/sh

if [ "$1" == "--help" ]; then
  echo "Usage: $0 [--help | dirname [number_of_images]]" >&2
  echo "dirname defaults to \"west\", number_of_images defaults to \"200\"" >&2
  exit
fi

FETCH="`dirname "$0"`/fetch.sh"

HOST=129.69.210.63
SIZE=640x480
AUTO=day
BACKL=0
BR=0
SAT=0
SHARP=0
NSUPP=high

DIR=west
[ ! -z "$1" ] && DIR=$1
TOTAL=200
[ ! -z "$2" ] && TOTAL=$2 

if [ -e "${DIR}" ]; then
  if [ ! -d "${DIR}" ]; then
    echo "A file named \"${DIR}\" exists. Specify different dirname." >&2
    exit 1
  else
    if [ -z "$1" ]; then
      echo "Default directory \"${DIR}\" exists already. Will not save files. Specify dirname explicitely to override." >&2 
      exit 1
    fi
  fi
else
  mkdir -p "${DIR}"
fi

for ((i=1;i<=TOTAL;++i)); do
  sh ${FETCH} $HOST $SIZE $AUTO $BR $BACKL $SAT $SHARP $NSUPP "${DIR}/west-norm-`date +%s.%N`.jpeg"
done
