set time
set title "ROC, SegmentMotion, Personen im Raum"
set xrange [0:200]
set yrange [0:200]
set xlabel "Fehlerrate (absolut)"
set ylabel "Trefferrate (absolut)"
set output "roc-motion-2.log.ps"
set terminal postscript
plot "roc-motion-2.log" using 1:2
