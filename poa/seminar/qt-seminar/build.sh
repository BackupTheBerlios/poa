# You need prosper and listings for teTeX
# to build this presentation!
latex qt-seminar.tex
dvips -o qt-seminar.ps qt-seminar.dvi
ps2pdf qt-seminar.ps
