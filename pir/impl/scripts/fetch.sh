#!/bin/sh
FETCH_CMD="wget -O $9"

function usage() {
    echo "fetch <host> <size 640x480,320x240,160x120>"
    echo "      <automatic off,auto,day,night,inactiv,autoonly>"
    echo "      <brightness -10..10> <backlight -10..10>"
    echo "      <saturation -10..10> <sharpen -2..20>"
    echo "      <noisesuppress off,medium,high> <output-file>"
}

if [ "$#" == 9 ]; then
    $FETCH_CMD http://$1/cgi-bin/image.jpg?camera=right&size=$2&automatic=$3&brightness=$4&backlight=$5&saturation=$6&sharpen=$7&darknoisesuppress=$8&date=0&textdisplay=disable
else
    usage;
fi
