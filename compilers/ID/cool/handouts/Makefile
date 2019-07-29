.IGNORE:

TEXSOURCES = asn1 asn2 asn3 asn4 asn5 \
	cool-manual cool-tour instructors-guide
INCLUDES = mymargins.tex macros.tex handout.sty

PS = $(foreach f, ${TEXSOURCES}, ${f}.ps)
PDF = $(foreach f, ${TEXSOURCES}, ${f}.pdf)

all: ${PS} ${PDF}

%.dvi : %.tex  ${INCLUDES}
	latex $<
	latex $<

%.ps : %.dvi
	rm -f $@
	dvips -t letter $< -o $@

%.pdf : %.tex ${INCLUDES}
	pdflatex $<
	pdflatex $<

clean:
	rm -f *.txt *.dvi *.log *.aux *~

really-clean: clean
	rm -f *.ps *.pdf
