if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    add_subdirectory(unit)
endif()

add_subdirectory(tools)
add_subdirectory(acceptance)
add_subdirectory(profiling)
add_subdirectory(static)
