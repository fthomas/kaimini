file(GLOB Kaimini_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ../src/*.cpp)
list(REMOVE_ITEM Kaimini_SOURCES ../src/main.cpp)
set(TEST_SOURCES testall.cpp ${Kaimini_SOURCES})
set(TEST_LIBRARIES
    ${Boost_LIBRARIES}
    ${Minuit2_LIBRARIES}
    gsl gslcblas m)

include_directories(
    ../src
    ${Boost_INCLUDE_DIRS}
    ${Minuit2_INCLUDE_DIRS})
link_directories(
    ${Boost_LIBRARY_DIRS}
    ${Minuit2_LIBRARY_DIRS})

add_executable(testall ${TEST_SOURCES})
target_link_libraries(testall ${TEST_LIBRARIES})

add_custom_target(test ${CMAKE_CURRENT_BINARY_DIR}/testall DEPENDS testall)
configure_file(slha0.txt . COPYONLY)
configure_file(slha1.txt . COPYONLY)
