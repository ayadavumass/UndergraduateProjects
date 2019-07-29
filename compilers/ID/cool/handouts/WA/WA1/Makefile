.IGNORE:

TEXSOURCES = WA1 WA1-sol
INCLUDES = ../handout.sty

FIGURES = wa1-q2 wa1-q3 sol-wa1-q1 sol-wa1-q3

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
