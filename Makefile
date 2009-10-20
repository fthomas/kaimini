#
# Makefile for RPVFit
#

INTEL_LIBS     = /opt/intel/Compiler/11.1/056/lib/intel64
MINUIT_INCPATH = /usr/include/root
MINUIT_LIBS    = /usr/lib/root/5.18
SPHENO_DIR     = $(HOME)/susy/code/spheno/trunk

SPHENO_MODPATH = $(SPHENO_DIR)/include
SPHENO_LIB     = $(SPHENO_DIR)/lib/libSPheno.a

CXX = g++
F90 = ifort
#F90 = gfortran

CXXFLAGS = -g -Wall -Wextra
INCPATH  = -I$(MINUIT_INCPATH)
LIBS     = -L$(MINUIT_LIBS) -lMinuit2 -Wl,-rpath=$(MINUIT_LIBS) $(SPHENO_LIB)
DEFINES  = 

ifneq (,$(findstring ifort,$(F90)))
  F90FLAGS = -debug -warn all -warn errors
  LIBS    += -lifcore -limf -lm -lintlc -ldl -Wl,-rpath=$(INTEL_LIBS)
  MODPATH  = -module $(SPHENO_MODPATH)
else ifneq (,$(findstring gfortran,$(F90)))
  F90FLAGS = -g -Wall
  LIBS    += -lgfortran
  MODPATH  = -J$(SPHENO_MODPATH)
endif

SOURCES = src/main.cpp \
          src/slha.cpp \
          src/spheno.f90
OBJECTS = src/main.o \
          src/slha.o \
          src/spheno.o
TARGET  = input/rpvfit

all: $(TARGET)


### Implicit rules:

.SUFFIXES: .o .cpp .f90 .F90

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(INCPATH) $(DEFINES) -o "$@" "$<"

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
	diff -au $(SPHENO_DIR)/src/SPheno3.f90 src/spheno.f90 | colordiff

$(TARGET): spheno $(OBJECTS)
	$(CXX) -o $(TARGET) $(OBJECTS) $(LIBS)

clean:
	rm -f $(OBJECTS)

cleanall: clean
	rm -f $(TARGET) Messages.out SPheno*.out SPheno.spc
	$(MAKE) -C input/ clean

.PHONY: all spheno spheno_clean spheno_diff clean cleanall
