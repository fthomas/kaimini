#
# Makefile for Kaimini
#

MINUIT_INCPATH = /usr/include/root
MINUIT_LIBS    = /usr/lib/root/5.24

CXX      = g++
CXXFLAGS = -fPIC -g -Wall -Wextra -pedantic
INCPATH  = -I$(MINUIT_INCPATH) $(RCPPFLAGS)
LIBS     = -lgsl -lgslcblas -lm -L$(MINUIT_LIBS) -lMinuit2 -ldl \
           -Wl,-rpath=$(MINUIT_LIBS) $(RLDFLAGS)
DEFINES  = 

R_LIBS    = $(subst -L,,$(filter -L%,$(shell R CMD config --ldflags)))
RCPPFLAGS = $(shell R CMD config --cppflags) \
            $(shell echo 'Rcpp:::CxxFlags()' | R --vanilla --slave)
RLDFLAGS  = $(shell R CMD config --ldflags) \
            $(shell echo 'Rcpp:::LdFlags()'  | R --vanilla --slave) \
            $(foreach PATH,$(R_LIBS),-Wl,-rpath=$(PATH))

SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))
KAIMINI    = input/kaimini
KAIMINI_SO = input/kaimini.so

all: $(KAIMINI) $(KAIMINI_SO)


### Implicit rules:

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

$(KAIMINI): $(OBJECTS)
	$(CXX) -o "$@" $(OBJECTS) $(LIBS)

$(KAIMINI_SO): $(filter-out %main.o,$(OBJECTS))
	$(CXX) -o "$@" -shared $(filter-out %main.o,$(OBJECTS)) $(LIBS)


clean:
	rm -f $(OBJECTS)

cleanall: clean
	rm -f $(KAIMINI) $(KAIMINI_SO)
	rm -rf html/
	$(MAKE) -C input/ clean

set_version:
	if [ -d '.git' ] && [ -x "`which git 2>/dev/null`" ]; then \
	  VERSION=`git describe --tags | cut -b2-`; \
	  sed -i "s/kaimini_version = \".*\"/kaimini_version = \"$$VERSION\"/" \
	    src/kaimini.h; \
	  SUBST="PROJECT_NUMBER         = $$VERSION"; \
	  sed -i "s/^PROJECT_NUMBER.*=.*/$$SUBST/" \
	     Doxyfile; \
	fi

.PHONY: all clean cleanall set_version
