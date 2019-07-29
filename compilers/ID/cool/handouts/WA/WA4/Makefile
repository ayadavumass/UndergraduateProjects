.IGNORE:

TEXSOURCES = WA4 WA4-sol 
INCLUDES = ../handout.sty

FIGURES = wa4-q2 wa4-q2-sol wa4-q1-sol

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
