set time
set title "Meschliche Wahrnehmung, keine Person"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bild"
set ylabel "Anzahl von Personen"
set output "roc-line-no-probands.log.ps"
set terminal postscript
plot "roc-line-no-probands.log" using 2
set time
set title "Menschliche Wahrnehmung, keine Person"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bild"
set ylabel "Anzahl von Personen"
set output "roc-line-no-probands.log.ps"
set terminal postscript
plot "roc-line-no-probands.log" using 2
