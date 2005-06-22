set time
set title "CalcOpticalFlowPyrLK mit Threshold 5.0, Feature-Punkte I, keine Person im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bild"
set ylabel "Anzahl von Personen"
set output "pyramid-t50-f1-1.log.ps"
set terminal postscript
plot "pyramid-t50-f1-1.log" using 2
