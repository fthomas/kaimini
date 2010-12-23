#
# Makefile for Kaimini
#

MINUIT_INCPATH = /usr/local/include
MINUIT_LIBS    = /usr/local/lib

CXX      = g++
CXXFLAGS = -fPIC -O2 -g -Wall
INCPATH  = $(shell gsl-config --cflags) -I$(MINUIT_INCPATH)
LDFLAGS  = 
LIBS     = -lboost_filesystem-mt -lboost_program_options-mt \
           $(shell gsl-config --libs) \
           -L$(MINUIT_LIBS) -lMinuit2 -lgomp -Wl,-rpath=$(MINUIT_LIBS)
DEFINES  = 

ifneq (,$(findstring g++,$(CXX)))
  CXXFLAGS += -Wextra -pedantic -fopenmp
else ifneq (,$(foreach CMD,icc icpc,$(findstring $(CMD),$(CXX))))
  CXXFLAGS += -strict-ansi -Wcheck -wd383 -wd444 -wd981 -openmp
  LDFLAGS  += -openmp
endif

SRCDIR   = src
OBJDIR   = src/.obj
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(subst $(SRCDIR),$(OBJDIR),$(SOURCES:.cpp=.o))
KAIMINI  = sandbox/kaimini
POLLY    = sandbox/polly

all: $(KAIMINI) $(POLLY)


### Implicit rules:

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p "$(@D)"
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

$(KAIMINI): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o "$@" $(OBJECTS) $(LIBS)

gprof: CXXFLAGS += -pg
gprof: LDFLAGS  += -pg
gprof: $(KAIMINI)

$(POLLY): tools/polly.cpp
	$(CXX) $(CXXFLAGS) $(INCPATH) -Isrc -o "$@" "$<"

clean:
	rm -f $(OBJECTS)
	-rmdir $(OBJDIR)

cleanall: clean
	rm -f $(KAIMINI) $(POLLY)
	$(MAKE) -C sandbox/ clean

.PHONY: all clean cleanall gprof
