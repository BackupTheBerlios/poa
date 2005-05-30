set time
set title "Menschliche Wahrnehmung, Korrekte Anzahl Personen"
set xrange [0:200]
set yrange [0:10]
set xlabel "Bildsequenz"
set ylabel "Anzahl von Personen"
set output "roc-line-some-probands.log.ps"
set terminal postscript
plot "roc-line-some-probands.log" using 2
