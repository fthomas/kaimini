set(Kaimini_TARGET kaimini)
set(Kaimini_SOURCES
    kaimini.cpp
    main.cpp
    minuitdriver.cpp
    parameters.cpp
    simpleslhafit.cpp
    slhafit.cpp
    sphenofit.cpp)

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${Minuit2_INCLUDE_DIRS})
link_directories(${Minuit2_LIBRARY_DIRS})
set(Kaimini_LIBRARIES ${Boost_LIBRARIES} gsl gslcblas m Minuit2)
set(Kaimini_RPATHS ${Minuit2_LIBRARY_DIRS})

add_executable(${Kaimini_TARGET} ${Kaimini_SOURCES})
target_link_libraries(${Kaimini_TARGET} ${Kaimini_LIBRARIES})
set_target_properties(${Kaimini_TARGET} PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE)
