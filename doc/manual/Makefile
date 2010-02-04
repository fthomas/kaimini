all: kaimini.pdf

clean:
	rm -f *~ *.backup *.aux *.bbl *.blg *.dvi *.log *.nav *.out \
	  *.pdf *.snm *.toc *.vrb

kaimini.pdf: kaimini.tex
	for i in `seq 1 3`; do \
	  pdflatex "$<"; \
	done
