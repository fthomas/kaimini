set(Kaimini_TARGET kaimini)
set(Kaimini_SOURCES
    gsldriver.cpp
    kaimini.cpp
    main.cpp
    minuitdriver.cpp
    parameters.cpp
    simpleslhafit.cpp
    slhafit.cpp
    softsusyfit.cpp
    sphenofit.cpp
    suspectfit.cpp)
set(Kaimini_LIBRARIES
    ${Boost_FILESYSTEM_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    gsl gslcblas m
    Minuit2 gomp)
set(Kaimini_RPATHS ${Minuit2_LIBRARY_DIRS})

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${Minuit2_INCLUDE_DIRS})
link_directories(${Minuit2_LIBRARY_DIRS})

add_executable(${Kaimini_TARGET} ${Kaimini_SOURCES})
target_link_libraries(${Kaimini_TARGET} ${Kaimini_LIBRARIES})
set_target_properties(${Kaimini_TARGET} PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE)

install(TARGETS ${Kaimini_TARGET} DESTINATION bin)
