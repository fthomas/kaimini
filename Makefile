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

CXXFLAGS = -g -Wall -Wextra -pedantic
INCPATH  = -I$(MINUIT_INCPATH) $(RCPPFLAGS)
LIBS     = -L$(MINUIT_LIBS) -lMinuit2 -ldl -Wl,-rpath=$(MINUIT_LIBS) \
           $(SPHENO_LIB) -lgsl -lgslcblas -lm $(RLDFLAGS)
DEFINES  = 

R_LIBS    = $(subst -L,,$(filter -L%,$(shell R CMD config --ldflags)))
RCPPFLAGS = $(shell R CMD config --cppflags) \
            $(shell echo 'Rcpp:::CxxFlags()' | R --vanilla --slave)
RLDFLAGS  = $(shell R CMD config --ldflags) \
            $(shell echo 'Rcpp:::LdFlags()'  | R --vanilla --slave) \
            $(foreach PATH,$(R_LIBS),-Wl,-rpath=$(PATH))

ifneq (,$(findstring ifort,$(F90)))
  F90FLAGS = -debug -warn all -warn errors
  LIBS    += -lifcore -limf -lm -lintlc -Wl,-rpath=$(INTEL_LIBS)
  MODPATH  = -module $(SPHENO_MODPATH)

else ifneq (,$(findstring gfortran,$(F90)))
  F90FLAGS = -g -Wall -Wextra -pedantic
  LIBS    += -lgfortran
  MODPATH  = -J$(SPHENO_MODPATH)
endif

SOURCES     := $(wildcard src/*.cpp src/*.f90 src/*.F90)
OBJECTS     := $(SOURCES:.cpp=.o)
OBJECTS     := $(OBJECTS:.f90=.o)
OBJECTS     := $(OBJECTS:.F90=.o)
OBJECTS_PIC := $(filter-out %main.pic.o,$(OBJECTS:.o=.pic.o))
RPVFIT       = input/rpvfit
FISP_SO      = input/fisp.so

all: $(RPVFIT)
shared: $(FISP_SO)


### Implicit rules:

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"

%.o: %.f90
	$(F90) -c $(F90FLAGS) $(MODPATH) -o "$@" "$<"

%.o: %.F90
	$(F90) -c $(F90FLAGS) $(MODPATH) $(DEFINES) -o "$@" "$<"


%.pic.o: %.cpp
	$(CXX) -c -fPIC $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"

%.pic.o: %.f90
	$(F90) -c -fPIC $(F90FLAGS) $(MODPATH) -o "$@" "$<"

%.pic.o: %.F90
	$(F90) -c -fPIC $(F90FLAGS) $(MODPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

spheno:
	$(MAKE) -C $(SPHENO_DIR)/src F90=$(F90)

spheno_clean:
	$(MAKE) -C $(SPHENO_DIR) cleanall

spheno_diff:
	@diff -au $(SPHENO_DIR)/src/SPheno3.f90 src/spheno.f90 || true


$(RPVFIT): spheno $(OBJECTS)
	$(CXX) -o "$@" $(OBJECTS) $(LIBS)

$(FISP_SO): spheno $(OBJECTS_PIC)
	$(CXX) -o "$@" -shared $(OBJECTS_PIC) $(LIBS)


clean:
	rm -f $(OBJECTS) $(OBJECTS_PIC)

cleanall: clean
	rm -f $(RPVFIT) $(FISP_SO) Messages.out SPheno*.out SPheno.spc
	$(MAKE) -C input/ clean

.PHONY: all spheno spheno_clean spheno_diff clean cleanall
