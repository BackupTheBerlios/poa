set time
set title "ROC, keine Personen im Bild"
set xrange [200:200]
set yrange [200:200]
set xlabel "Fehlerrate (absolut)"
set ylabel "Trefferrate (absolut)"
set output "roc-optical-flow-1.log.ps"
set terminal postscript
plot "roc-optical-flow-1.log" using 1 2
