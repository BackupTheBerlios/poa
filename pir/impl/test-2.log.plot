set time
set title "Eigener Algorithmus, Personen im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "test-2.log.ps"
set terminal postscript
plot "test-2.log" using 2
