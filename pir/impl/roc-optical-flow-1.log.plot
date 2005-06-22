set time
set title "Eigener Algorithmus, Erkennungsrate, keine Personen"
set xrange [0:200]
set yrange [0:200]
set xlabel "Fehlerrate (absolut)"
set ylabel "Trefferrate (absolut)"
set output "roc-optical-flow-1.log.ps"
set terminal postscript
plot "roc-optical-flow-1.log" using 1:2
