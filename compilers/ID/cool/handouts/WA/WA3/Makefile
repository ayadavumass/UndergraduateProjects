.IGNORE:

TEXSOURCES = WA3
INCLUDES = ../handout.sty

FIGURES = lr-dfa

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
	rm -f *.txt *.dvi *.log *.aux *~

really-clean: clean
	rm -f *.ps *.pdf
