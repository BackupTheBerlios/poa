set time
set title "SegmentMotion History 3s, Buffer 2, Min 0.01, Max 1, Personen im Bild"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "motion-2-d3m001M1.log.ps"
set terminal postscript
plot "motion-2-d3m001M1.log" using 2
