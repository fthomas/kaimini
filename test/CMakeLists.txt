set(TEST_SOURCES
    testall.cpp
    ../src/kaimini.cpp
    ../src/parameters.cpp
    ../src/simpleslhafit.cpp
    ../src/slhafit.cpp
    ../src/sphenofit.cpp)

include_directories(
    ../src
    ${Minuit2_INCLUDE_DIRS})
link_directories(${Minuit2_LIBRARY_DIRS})

add_executable(testall ${TEST_SOURCES})
target_link_libraries(testall cppunit ${Boost_LIBRARIES} gsl gslcblas Minuit2)

add_custom_target(test ${CMAKE_CURRENT_BINARY_DIR}/testall DEPENDS testall)
configure_file(slha0.txt . COPYONLY)
configure_file(slha1.txt . COPYONLY)
