get_target_property(kaimini_SOURCES      kaimini kaimini_SOURCES)
get_target_property(kaimini_INCLUDE_DIRS kaimini kaimini_INCLUDE_DIRS)
get_target_property(kaimini_LIBRARY_DIRS kaimini kaimini_LIBRARY_DIRS)
get_target_property(kaimini_LIBRARIES    kaimini kaimini_LIBRARIES)

list(REMOVE_ITEM kaimini_SOURCES ${CMAKE_SOURCE_DIR}/src/kernel/Kaimini.cpp)

include_directories(${CMAKE_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS}
  ${kaimini_INCLUDE_DIRS})

link_directories(${kaimini_LIBRARY_DIRS})

file(GLOB_RECURSE ut_SOURCES *.cpp *.h)
add_executable(ut ${ut_SOURCES} ${kaimini_SOURCES})
target_link_libraries(ut ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
  ${kaimini_LIBRARIES})

if(CMAKE_COMPILER_IS_GNUCXX)
    set_target_properties(ut PROPERTIES
      COMPILE_FLAGS "--coverage -g -O0"
      LINK_FLAGS    "--coverage")

    find_program(COVERAGE_LCOV lcov)
    find_program(COVERAGE_GENHTML genhtml)
    if(COVERAGE_LCOV AND COVERAGE_GENHTML)
        set(COVERAGE_OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/coverage.info)
        add_custom_target(lcov
          COMMAND ${COVERAGE_LCOV}
            --zerocounters
            --directory ${CMAKE_CURRENT_BINARY_DIR}
          COMMAND ut
          COMMAND ${COVERAGE_LCOV}
            --capture
            --directory ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/ut.dir/
            --base-directory ${CMAKE_SOURCE_DIR}
            --output-file ${COVERAGE_OUTFILE}
          COMMAND ${COVERAGE_LCOV}
            --extract ${COVERAGE_OUTFILE} "${CMAKE_SOURCE_DIR}/src/\\*"
            --output-file ${COVERAGE_OUTFILE}
          COMMAND ${COVERAGE_GENHTML}
            --output-directory ${CMAKE_CURRENT_SOURCE_DIR}/lcov-report
            --prefix ${CMAKE_SOURCE_DIR}
            --legend --demangle-cpp --title ${CMAKE_PROJECT_NAME}
            ${COVERAGE_OUTFILE})
    endif()
endif()

add_test(NAME unit_tests COMMAND ut)
