include_directories(../src)

add_executable(testall testall.cpp)
target_link_libraries(testall cppunit)

add_custom_target(test ${CMAKE_CURRENT_BINARY_DIR}/testall DEPENDS testall)
