# Path to the SPheno source code, modules, and library. You probably need to
# adjust SPHENO_DIR.
SPHENO_DIR     = $(HOME)/susy/code/spheno/trunk
SPHENO_MODPATH = $(SPHENO_DIR)/include
SPHENO_LIB     = $(SPHENO_DIR)/lib/libSPheno.a

CXX = g++
F90 = ifort
#F90 = gfortran

CXXFLAGS = -g -Wall -Wextra

ifneq (,$(findstring ifort,$(F90)))
  # Set compiler flags for ifort.
  FFLAGS   = -debug -warn all -warn errors
  MODPATH  = -module $(SPHENO_MODPATH)
  FORTLIBS = -lifcore -limf -lm -lintlc -ldl \
             -Wl,-rpath=/opt/intel/Compiler/11.1/046/lib/intel64

else ifneq (,$(findstring gfortran,$(F90)))
  # Set compiler flags for gfortran.
  FFLAGS   = -g -Wall
  MODPATH  = -J$(SPHENO_MODPATH)
  FORTLIBS = -lgfortran
endif

DEFINES = 

SOURCES = Auxiliary.f90 \
          ChiSquare.f90 \
          MimicSPheno.f90 \
          main.cpp
OBJECTS = Auxiliary.o \
          ChiSquare.o \
          MimicSPheno.o \
          main.o
TARGET  = chisquare

all: spheno $(TARGET)

### Implicit rules:

.SUFFIXES: .o .cpp .f90 .F90

.cpp.o:
	$(CXX) -c $(CXXFLAGS) $(DEFINES) -o "$@" "$<"

.f90.o:
	$(F90) -c $(FFLAGS) $(MODPATH) -o "$@" "$<"

.F90.o:
	$(F90) -c $(FFLAGS) $(MODPATH) $(DEFINES) -o "$@" "$<"

### Build and clean rules:

spheno:
	$(MAKE) -C $(SPHENO_DIR)/src F90=$(F90)

spheno_clean:
	$(MAKE) -C $(SPHENO_DIR) cleanall

$(TARGET): $(OBJECTS)
	$(CXX) $(FORTLIBS) -o $(TARGET) $(OBJECTS) $(SPHENO_LIB)

clean:
	rm -f $(OBJECTS)

cleanall: clean
	rm -f $(TARGET) Messages.out SPheno.out SPheno.spc

.PHONY: all spheno spheno_clean clean cleanall
