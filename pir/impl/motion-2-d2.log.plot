set time
set title "SegmentMotion History 2s, Buffer 2, Personen im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "motion-2-d2.log.ps"
set terminal postscript
plot "motion-2-d2.log" using 2
