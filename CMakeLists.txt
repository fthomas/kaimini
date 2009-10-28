cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

project(FISP CXX Fortran)

get_filename_component(Fortran_COMPILER ${CMAKE_Fortran_COMPILER} NAME)

find_program(MAKE NAMES make gmake)
find_package(Boost REQUIRED)
find_package(Minuit2 REQUIRED)
find_package(SPheno REQUIRED)

add_custom_target(spheno ${MAKE}
    WORKING_DIRECTORY ${SPheno_ROOT_DIR}/src)
add_custom_target(spheno_clean ${MAKE} cleanall
    WORKING_DIRECTORY ${SPheno_ROOT_DIR})

add_subdirectory(src ${CMAKE_BUILD_DIR}/..)
