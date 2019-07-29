.IGNORE:

TEXSOURCES = WA7 WA7-sol
INCLUDES = ../handout.sty

FIGURES = objects
FIGSCALE = 0.800000

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

%.eps: %.fig
	fig2dev -L pstex -m ${FIGSCALE} -n $*.pstex $*.fig $*.pstex
	fig2dev -L pstex_t -p $*.pstex -m ${FIGSCALE} $*.fig $*.pstex_t
	./pstex-to-eps $*.pstex_t

clean:
	rm -f *.txt *.dvi *.log *.aux *.ps *.pdf *.pstex *.pstex_t

really-clean: clean
	rm -f *.ps *.pdf
