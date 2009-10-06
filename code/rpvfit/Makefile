#
#  Makefile for RPVfit
#

INTEL_LIBS     = /opt/intel/Compiler/11.1/056/lib/intel64
SPHENO_DIR     = $(HOME)/susy/code/spheno/trunk

SPHENO_MODPATH = $(SPHENO_DIR)/include
SPHENO_LIB     = $(SPHENO_DIR)/lib/libSPheno.a


CXX = g++
F90 = ifort
#F90 = gfortran

CXXFLAGS = -g -Wall -Wextra

ifneq (,$(findstring ifort,$(F90)))
  F90FLAGS = -debug -warn all -warn errors
  F90LIBS  = -lifcore -limf -lm -lintlc -ldl -Wl,-rpath=$(INTEL_LIBS)
  MODPATH  = -module $(SPHENO_MODPATH)
else ifneq (,$(findstring gfortran,$(F90)))
  F90FLAGS = -g -Wall
  F90LIBS  = -lgfortran
  MODPATH  = -J$(SPHENO_MODPATH)
endif

DEFINES = 

SOURCES = SPhenoDouble.f90 \
          main.cpp
OBJECTS = SPhenoDouble.o \
          main.o
TARGET  = rpvfit

all: $(TARGET)


### Implicit rules:

.SUFFIXES: .o .cpp .f90 .F90

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(DEFINES) -o "$@" "$<"

.f90.o:
	$(F90) -c $(F90FLAGS) $(MODPATH) -o "$@" "$<"

.F90.o:
	$(F90) -c $(F90FLAGS) $(MODPATH) $(DEFINES) -o "$@" "$<"


### Build and clean rules:

spheno:
	$(MAKE) -C $(SPHENO_DIR)/src F90=$(F90)

spheno_clean:
	$(MAKE) -C $(SPHENO_DIR) cleanall

spheno_diff:
	diff -au $(SPHENO_DIR)/src/SPheno3.f90 SPhenoDouble.f90 | colordiff

$(TARGET): spheno $(OBJECTS)
	$(CXX) $(F90LIBS) -o $(TARGET) $(OBJECTS) $(SPHENO_LIB)

clean:
	rm -f $(OBJECTS)

cleanall: clean
	rm -f $(TARGET) Messages.out SPheno*.out SPheno.spc

.PHONY: all spheno spheno_clean clean cleanall
