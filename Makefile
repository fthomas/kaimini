#
# Makefile for FISP
#

INTEL_LIBS     = /opt/intel/Compiler/11.1/056/lib/intel64
MINUIT_INCPATH = /usr/include/root
MINUIT_LIBS    = /usr/lib/root/5.24
SPHENO_DIR     = $(HOME)/susy/code/spheno/trunk

SPHENO_MODPATH = $(SPHENO_DIR)/include
SPHENO_LIB     = $(SPHENO_DIR)/lib/libSPheno.a

CXX = g++
F90 = ifort
#F90 = gfortran

CXXFLAGS = -fPIC -g -Wall -Wextra -pedantic
INCPATH  = -I$(MINUIT_INCPATH) $(RCPPFLAGS)
LIBS     = $(F90LIBS) -lgsl -lgslcblas -lm -L$(MINUIT_LIBS) -lMinuit2 -ldl \
           -Wl,-rpath=$(MINUIT_LIBS) $(SPHENO_LIB) $(RLDFLAGS)
DEFINES  = 

R_LIBS    = $(subst -L,,$(filter -L%,$(shell R CMD config --ldflags)))
RCPPFLAGS = $(shell R CMD config --cppflags) \
            $(shell echo 'Rcpp:::CxxFlags()' | R --vanilla --slave)
RLDFLAGS  = $(shell R CMD config --ldflags) \
            $(shell echo 'Rcpp:::LdFlags()'  | R --vanilla --slave) \
            $(foreach PATH,$(R_LIBS),-Wl,-rpath=$(PATH))

ifneq (,$(findstring ifort,$(F90)))
  F90FLAGS = -fPIC -debug -warn all -warn errors
  F90LIBS  = -lifcore -limf -lm -lintlc -Wl,-rpath=$(INTEL_LIBS)
  MODPATH  = -module $(SPHENO_MODPATH)

else ifneq (,$(findstring gfortran,$(F90)))
  F90FLAGS = -fPIC -g -Wall -Wextra -pedantic
  F90LIBS  = -lgfortran
  MODPATH  = -J$(SPHENO_MODPATH)
endif

SOURCES := $(wildcard src/*.cpp src/*.f90 src/*.F90)
OBJECTS := $(addsuffix .o,$(basename $(SOURCES)))
RPVFIT   = input/rpvfit
FISP_SO  = input/fisp.so

all: set_version $(RPVFIT) $(FISP_SO)


### Implicit rules:

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"

%.o: %.f90
	$(F90) -c $(F90FLAGS) $(MODPATH) -o "$@" "$<"

%.o: %.F90
	$(F90) -c $(F90FLAGS) $(MODPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

spheno:
	$(MAKE) -C $(SPHENO_DIR)/src F90=$(F90)

spheno_clean:
	$(MAKE) -C $(SPHENO_DIR) cleanall

spheno_diff:
	@diff -au $(SPHENO_DIR)/src/SPheno3.f90 src/spheno.f90 || true


$(RPVFIT): spheno $(OBJECTS)
	$(CXX) -o "$@" $(OBJECTS) $(LIBS)

$(FISP_SO): spheno $(filter-out %main.o,$(OBJECTS))
	$(CXX) -o "$@" -shared $(filter-out %main.o,$(OBJECTS)) $(LIBS)


clean:
	rm -f $(OBJECTS)

cleanall: clean
	rm -f $(RPVFIT) $(FISP_SO)
	$(MAKE) -C input/ clean

set_version:
	if [ -d '.git' ] && [ -x "`which git 2>/dev/null`" ]; then \
	  VERSION=`git describe --tags | cut -b2-`; \
	  sed -i "s/fisp_version = \".*\"/fisp_version = \"$$VERSION\"/" \
	    src/fisp.h; \
	fi

.PHONY: all shared spheno spheno_clean spheno_diff clean cleanall set_version
