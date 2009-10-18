clean:
	rm -f Messages.out SPheno*.out SPheno.spc

switch:
	TARGET=`readlink -n LesHouches.in`; \
	rm LesHouches.in; \
	if [ "$$TARGET" == "input.MSSM" ]; then \
	  ln -s input.mSUGRA LesHouches.in; \
	else \
	  ln -s input.MSSM   LesHouches.in; \
	fi

.PHONY: clean
