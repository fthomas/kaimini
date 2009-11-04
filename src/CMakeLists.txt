set(RPVFit_TARGET rpvfit)
set(RPVFit_SOURCES
    main.cpp
    fcn.cpp
    parameters.cpp
    slha.cpp
    spheno.f90)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${Minuit2_INCLUDE_DIRS}
    ${SPheno_INCLUDE_DIRS})
link_directories(${Minuit2_LIBRARY_DIRS})
set(RPVFit_LIBRARIES gsl gslcblas m Minuit2 ${SPheno_LIBRARIES})
set(RPVFit_RPATHS ${Minuit2_LIBRARY_DIRS})

if(Fortran_COMPILER MATCHES ifort)
    find_library(ifort_LIBRARY_DIR ifcore HINTS ENV LIBRARY_PATH)
    get_filename_component(ifort_LIBRARY_DIR ${ifort_LIBRARY_DIR} PATH)

    link_directories(${ifort_LIBRARY_DIR})
    set(RPVFit_LIBRARIES ${RPVFit_LIBRARIES} ifcore imf intlc dl)
    set(RPVFit_RPATHS "${RPVFit_RPATHS}\;${ifort_LIBRARY_DIR}")

elseif(Fortran_COMPILER MATCHES gfortran)
    set(RPVFit_LIBRARIES ${RPVFit_LIBRARIES} gfortran)
endif()

add_executable(${RPVFit_TARGET} ${RPVFit_SOURCES})
add_dependencies(${RPVFit_TARGET} spheno)
target_link_libraries(${RPVFit_TARGET} ${RPVFit_LIBRARIES})
set_target_properties(${RPVFit_TARGET} PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE)
