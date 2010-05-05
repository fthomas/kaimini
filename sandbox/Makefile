kaimini:
	$(MAKE) -C ..

run:
	kaimini -i kaimini.in -o kaimini.out
	kaimini -i polly.in   -o polly.out

clean:
	rm -rf .kaimini-*

cleanall: clean
	$(MAKE) -C .. cleanall

.PHONY: kaimini run clean cleanall
