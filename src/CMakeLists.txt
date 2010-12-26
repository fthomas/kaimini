add_subdirectory(kernel)

file(GLOB_RECURSE kaimini_SOURCES *.cpp *.h)

set(kaimini_LIBRARIES
    ${Boost_FILESYSTEM_LIBRARIES} ${Boost_PROGRAM_OPTIONS_LIBRARIES}
    ${Minuit2_LIBRARIES}
    gsl gslcblas m)
set(Kaimini_RPATHS
    ${Boost_LIBRARY_DIRS}
    ${Minuit2_LIBRARY_DIRS})

set(kaimini_INCLUDE_DIRS
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
    ${Minuit2_INCLUDE_DIRS})
include_directories( ${kaimini_INCLUDE_DIRS})

set(kaimini_LIBRARY_DIRS
    ${Boost_LIBRARY_DIRS}
    ${Minuit2_LIBRARY_DIRS})

link_directories(${kaimini_LIBRARY_DIRS})

add_executable(kaimini ${kaimini_SOURCES})
target_link_libraries(kaimini ${kaimini_LIBRARIES})
set_target_properties(kaimini PROPERTIES
    INSTALL_RPATH_USE_LINK_PATH TRUE
    BUILD_WITH_INSTALL_RPATH TRUE)

set_target_properties(kaimini PROPERTIES
    kaimini_SOURCES   "${kaimini_SOURCES}"
    kaimini_INCLUDE_DIRS "${kaimini_INCLUDE_DIRS}"
    kaimini_LIBRARY_DIRS "${kaimini_LIBRARY_DIRS}"
    kaimini_LIBRARIES "${kaimini_LIBRARIES}")

install(TARGETS kaimini DESTINATION bin)
