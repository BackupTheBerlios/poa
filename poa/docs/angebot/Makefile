###############################################################################
## Makefile for generating Angebot papers
## $Id: Makefile,v 1.1 2003/05/31 16:16:29 garbeam Exp $

TEXCMD=latex
PDFCMD=pdflatex
PSCMD=dvips
PSCMDARGS=-o
INPUT=angebot
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
