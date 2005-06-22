#!/bin/sh
echo set time>>$2.plot
echo set title \"$1\">>$2.plot
echo set xrange [0:200]>>$2.plot
echo set yrange [0:10]>>$2.plot
echo set xlabel \"Bild\">>$2.plot
echo set ylabel \"Anzahl von Personen\">>$2.plot
echo set output \"$2.ps\">>$2.plot
echo set terminal postscript>>$2.plot
echo plot \"$2\" using 2 >>$2.plot
gnuplot $2.plot
