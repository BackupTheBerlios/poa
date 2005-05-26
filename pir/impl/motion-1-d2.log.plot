set time
set title "SegmentMotion History 2s, Buffer 2, keine Person im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "motion-1-d2.log.ps"
set terminal postscript
plot "motion-1-d2.log" using 2
