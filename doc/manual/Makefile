all: kaimini.pdf

clean:
	rm -f *~ *.backup *.aux *.bbl *.blg *.dvi *.log *.nav *.out \
	  *.pdf *.snm *.toc *.vrb

kaimini.pdf: kaimini.tex
	pdflatex "$<"
	bibtex "$(<:.tex=)"
	pdflatex "$<"
	pdflatex "$<"
