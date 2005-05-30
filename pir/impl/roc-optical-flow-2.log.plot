set time
set title "ROC, verschiedene Personen"
set xrange [0:200]
set yrange [0:200]
set xlabel "Fehlerrate (absolut)"
set ylabel "Trefferrate (absolut)"
set output "roc-optical-flow-2.log.ps"
set terminal postscript
plot "roc-optical-flow-2.log" using 1:2
