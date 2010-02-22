#
# Makefile for Kaimini
#

MINUIT_INCPATH = /usr/local/include
MINUIT_LIBS    = /usr/local/lib

CXX      = g++
CXXFLAGS = -fPIC -O2 -g -Wall

ifneq (,$(findstring g++,$(CXX)))
  CXXFLAGS += -Wextra -pedantic
else ifneq (,$(findstring icc,$(CXX)))
  CXXFLAGS += -strict-ansi -Wcheck -wd383 -wd981
endif

INCPATH  = $(shell gsl-config --cflags) -I$(MINUIT_INCPATH)
LIBS     = -lboost_filesystem-mt -lboost_program_options-mt \
           $(shell gsl-config --libs) \
           -L$(MINUIT_LIBS) -lMinuit2 -lgomp -Wl,-rpath=$(MINUIT_LIBS)
DEFINES  = 

SRCDIR   = src
OBJDIR   = src/.obj
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(subst $(SRCDIR),$(OBJDIR),$(SOURCES:.cpp=.o))
KAIMINI    = input/kaimini
KAIMINI_SO = input/kaimini.so

all: $(KAIMINI)


### Implicit rules:

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

$(KAIMINI): $(OBJECTS)
	$(CXX) -o "$@" $(OBJECTS) $(LIBS)

$(KAIMINI_SO): $(filter-out %main.o,$(OBJECTS))
	$(CXX) -o "$@" -shared $(filter-out %main.o,$(OBJECTS)) $(LIBS)


doc:
	doxygen doc/Doxyfile
	$(MAKE) -C doc/manual/

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
	  V_MAJOR=`echo $$VERSION | cut -d. -f1`; \
	  V_MINOR=`echo $$VERSION | cut -d. -f2`; \
	  V_PATCH=`echo $$VERSION | cut -d. -f3`; \
	  sed -i \
	    "s/^\(set(Kaimini_VERSION_MAJOR \).*/\1\"$$V_MAJOR\")/; \
	     s/^\(set(Kaimini_VERSION_MINOR \).*/\1\"$$V_MINOR\")/; \
	     s/^\(set(Kaimini_VERSION_PATCH \).*/\1\"$$V_PATCH\")/" \
	    CMakeLists.txt; \
	  sed -i "s/^\(PROJECT_NUMBER.*= \).*/\1$$VERSION/" doc/Doxyfile; \
	  sed -i "s/\(g_kaimini_version = \)\".*\"/\1\"$$VERSION\"/" \
	    src/kaimini.h; \
	fi

.PHONY: all doc clean cleanall set_version
