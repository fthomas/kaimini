#
# Makefile for Kaimini
#

MINUIT_INCPATH = /usr/local/include
MINUIT_LIBS    = /usr/local/lib

CXX      = g++
CXXFLAGS = -fPIC -g -Wall -Wextra -pedantic
INCPATH  = $(shell gsl-config --cflags) -I$(MINUIT_INCPATH)
LIBS     = -lboost_filesystem-mt $(shell gsl-config --libs) \
           -L$(MINUIT_LIBS) -lMinuit2 -Wl,-rpath=$(MINUIT_LIBS)
DEFINES  = 

SRCDIR  := src
OBJDIR  := src/.obj
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(subst $(SRCDIR),$(OBJDIR),$(SOURCES:.cpp=.o))
KAIMINI    = input/kaimini
KAIMINI_SO = input/kaimini.so

all: $(KAIMINI) $(KAIMINI_SO)


### Implicit rules:

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

$(KAIMINI): $(OBJECTS)
	$(CXX) -o "$@" $(OBJECTS) $(LIBS)

$(KAIMINI_SO): $(filter-out %main.o,$(OBJECTS))
	$(CXX) -o "$@" -shared $(filter-out %main.o,$(OBJECTS)) $(LIBS)


clean:
	rm -f $(OBJECTS)
	-rmdir $(OBJDIR)

cleanall: clean
	rm -f $(KAIMINI) $(KAIMINI_SO)
	rm -rf doc/html/
	$(MAKE) -C doc/manual/ clean
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
