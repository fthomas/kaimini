set(TEST_SOURCES
    testall.cpp
    ../src/kaimini.cpp
    ../src/minuitdriver.cpp
    ../src/parameters.cpp
    ../src/simpleslhafit.cpp
    ../src/slhafit.cpp
    ../src/softsusyfit.cpp
    ../src/sphenofit.cpp)
set(TEST_LIBRARIES
    ${Boost_LIBRARIES}
    gsl gslcblas m
    Minuit2 gomp)

include_directories(
    ../src
    ${Boost_INCLUDE_DIRS}
    ${Minuit2_INCLUDE_DIRS})
link_directories(${Minuit2_LIBRARY_DIRS})

add_executable(testall ${TEST_SOURCES})
target_link_libraries(testall ${TEST_LIBRARIES})

add_custom_target(test ${CMAKE_CURRENT_BINARY_DIR}/testall DEPENDS testall)
configure_file(slha0.txt . COPYONLY)
configure_file(slha1.txt . COPYONLY)
