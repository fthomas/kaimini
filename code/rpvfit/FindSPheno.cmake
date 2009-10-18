
set(SPheno_POSSIBLE_DIRS
    ~/susy/code/spheno/trunk
    ~/SPheno
    ~/spheno
    ~/src/SPheno
    ~/src/spheno
    /usr/src/SPheno
    /usr/src/spheno
    /usr/local/src/SPheno
    /usr/local/src/spheno
    /opt/SPheno
    /opt/spheno)

find_path(SPheno_ROOT_DIR src/SPheno3.f90 PATHS ${SPheno_POSSIBLE_DIRS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(SPheno DEFAULT_MSG SPheno_ROOT_DIR)

set(SPheno_INCLUDE_DIRS ${SPheno_ROOT_DIR}/include)
set(SPheno_LIBRARIES    ${SPheno_ROOT_DIR}/lib/libSPheno.a)
set(SPheno_EXECUTABLE   ${SPheno_ROOT_DIR}/bin/SPheno)

mark_as_advanced(
    SPheno_ROOT_DIR
    SPheno_INCLUDE_DIRS
    SPheno_LIBRARIES
    SPheno_EXECUTABLE)
