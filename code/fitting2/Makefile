# Path to the SPheno source code, modules, and library. You probably need to
# adjust SPHENO_DIR.
SPHENO_DIR = $(HOME)/susy/code/spheno/trunk
SPHENO_MODPATH = $(SPHENO_DIR)/include
SPHENO_LIB = $(SPHENO_DIR)/lib/libSPheno.a

# Use ifort for compilation:
F90     = ifort
FFLAGS  = -debug -warn all -warn errors
MODPATH = -module $(SPHENO_MODPATH)

# Use gfortran for compilation:
#F90     = gfortran
#FFLAGS  = -g -Wall
#MODPATH = -J$(SPHENO_MODPATH)

DEFINES = 

SOURCES = Auxiliary.f90 \
          ChiSquare.f90
OBJECTS = Auxiliary.o \
          ChiSquare.o
TARGET  = chisquare

all: spheno $(TARGET)

### Implicit rules:

.SUFFIXES: .o .f90 .F90

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
	$(F90) -o $(TARGET) $(OBJECTS) $(SPHENO_LIB)

clean:
	rm -f $(OBJECTS)

.PHONY: all spheno spheno_clean clean
