set time
set title "SegmentMotion History 10s, Buffer 2, Min 0.01, Max 3, keine Person im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bild"
set ylabel "Anzahl von Personen"
set output "motion-1-d10m001M3.log.ps"
set terminal postscript
plot "motion-1-d10m001M3.log" using 2
