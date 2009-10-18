set(RPVFit_TARGET rpvfit)
set(RPVFit_SOURCES
    main.cpp
    slha.cpp
    spheno.f90)

include_directories(
    ${SPheno_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS})

add_executable(${RPVFit_TARGET} ${RPVFit_SOURCES})
add_dependencies(${RPVFit_TARGET} spheno)
target_link_libraries(${RPVFit_TARGET} ${SPheno_LIBRARIES})

if(Fortran_COMPILER MATCHES ifort)
    find_library(ifort_LIBRARY_PATH NAMES ifcore HINTS ENV LIBRARY_PATH)
    get_filename_component(ifort_LIBRARY_PATH ${ifort_LIBRARY_PATH} PATH)

    link_directories(${ifort_LIBRARY_PATH})
    target_link_libraries(${RPVFit_TARGET} ifcore imf intlc dl)
    set_target_properties(${RPVFit_TARGET} PROPERTIES
        INSTALL_RPATH ${ifort_LIBRARY_PATH}
        BUILD_WITH_INSTALL_RPATH TRUE)

elseif(Fortran_COMPILER MATCHES gfortran)
    target_link_libraries(${RPVFit_TARGET} gfortran)
endif()
