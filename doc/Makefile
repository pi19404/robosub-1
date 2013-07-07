BASE_NAME=proposal

all: pdf clean_non_pdf

# Running twice because citations frequently won't work
# unless the compilation is run twice.
pdf:
	pdflatex $(BASE_NAME).tex
	pdflatex $(BASE_NAME).tex

clean_non_pdf:
	$(RM) $(BASE_NAME).aux
	$(RM) $(BASE_NAME).log

clean: clean_non_pdf
	$(RM) $(BASE_NAME).pdf


