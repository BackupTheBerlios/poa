set time
set title "ROC, SegmentMotion, keine Person"
set xrange [0:200]
set yrange [0:200]
set xlabel "Fehlerrate (absolut)"
set ylabel "Trefferrate (absolut)"
set output "roc-motion-1.log.ps"
set terminal postscript
plot "roc-motion-1.log" using 1:2
