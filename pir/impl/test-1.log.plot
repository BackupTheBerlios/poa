set time
set title "Eigener Algorithmus, keine Person im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bild"
set ylabel "Anzahl von Personen"
set output "test-1.log.ps"
set terminal postscript
plot "test-1.log" using 2
