cmake_minimum_required(VERSION 2.6)
set(CMAKE_VERBOSE_MAKEFILE true)

project(RPVFit CXX Fortran)

message(STATUS "$ENV{HOME}/susy/code/spheno/trunk")
#$ENV{HOME}/SPheno
#$ENV{HOME}/spheno
#/usr/src/SPheno
#/usr/src/spheno
#/usr/local/src/SPheno
#/usr/local/src/spheno

find_package(Boost REQUIRED)

##${Boost_INCLUDE_DIRS}

#enable_language(Fortran)

#add_executable(rpvfit main.cpp slha.cpp spheno.f90)

#include(FortranCInterface)
