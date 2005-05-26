set time
set title "CalcOpticalFlowPyrLK mit Threshold 1.0, Feature-Punkte I, keine Person im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "pyramid-f1-1.log.ps"
set terminal postscript
plot "pyramid-f1-1.log" using 2
