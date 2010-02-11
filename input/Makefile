kaimini:
	$(MAKE) -C ..

clean:
	rm -f kaimini.out
	rm -rf .kaimini-*

switch:
	TARGET=`readlink -n kaimini.in`; \
	rm kaimini.in; \
	if [ "$$TARGET" == "input.MSSM" ]; then \
	  ln -s input.mSUGRA kaimini.in; \
	else \
	  ln -s input.MSSM   kaimini.in; \
	fi

.PHONY: kaimini clean switch
