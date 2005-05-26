set time
set title "CalcOpticalFlowPyrLK mit Threshold 10.0, Feature-Punkte II, keine Person im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "pyramid-t100-f2-1.log.ps"
set terminal postscript
plot "pyramid-t100-f2-1.log" using 2
