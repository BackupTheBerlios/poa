set time
set title "ROC, SegmentMotion, Testdurchlaeufe"
set xrange [0:100]
set yrange [0:100]
set xlabel "Fehlerrate (in Prozent)"
set ylabel "Trefferrate (in Prozent)"
set output "roc-motion.ps"
set terminal postscript
plot "roc_segment_motion" using 1:4 with lines
