SOURCE=arch

all: source clean

clean:
	- rm *.aux *.log 
	# beamer:
	#- rm *.nav *.snm *.toc *.out *.log *.aux

source:
	pdflatex $(SOURCE).tex
	evince $(SOURCE).pdf


