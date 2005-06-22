set time
set title "ROC, CalcOpticalFlowPyr, Testdurchlaeufe"
set xrange [0:100]
set yrange [0:100]
set xlabel "Fehlerrate (in Prozent)"
set ylabel "Trefferrate (in Prozent)"
set output "roc-pyramid.ps"
set terminal postscript
plot "roc_pyramid" using 1:4 with lines
