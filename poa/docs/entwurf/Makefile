###############################################################################
## Makefile for generating Entwurf papers
## $Id: Makefile,v 1.1 2003/07/03 08:27:02 garbeam Exp $

TEXCMD=latex
PDFCMD=pdflatex
PSCMD=dvips
PSCMDARGS=-o
INPUT=entwurf
TEXEXT=.tex
DVIEXT=.dvi
PSEXT=.ps

start: dvi

clean:
	rm -f *.pdf *.ps *.dvi *.toc *.aux *.log

all: clean dvi pub end

pub:
	$(PSCMD) $(PSCMDARGS) $(INPUT)$(PSEXT) $(INPUT)$(DVIEXT)
	$(PDFCMD) $(INPUT)$(TEXEXT)

dvi:
	$(TEXCMD) $(INPUT)$(TEXEXT)

end:
