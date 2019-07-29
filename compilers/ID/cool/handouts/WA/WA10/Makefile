.IGNORE:

TEXSOURCES = WA10 WA10-sol 
INCLUDES = ../handout.sty

FIGURES = wa10-s1a wa10-s1b wa10-s1c wa10-s2a wa10-s2b wa10-s2c

PS = $(foreach f, ${TEXSOURCES}, ${f}.ps)
PDF = $(foreach f, ${TEXSOURCES}, ${f}.pdf)

all: ${PS} ${PDF}

%.dvi : %.tex  ${INCLUDES} ${FIGURES:%=%.eps}
	latex $<
	latex $<

%.ps : %.dvi
	rm -f $@
	dvips -t letter $< -o $@

%.pdf : %.tex ${INCLUDES} ${FIGURES:%=%.pdf}
	pdflatex $<
	pdflatex $<

%.pdf: %.eps
	epstopdf $<

clean:
	rm -f *.txt *.dvi *.log *.aux *.ps *.pdf

really-clean: clean
	rm -f *.ps *.pdf
