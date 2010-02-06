cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

project(Kaimini CXX)

# Enable more warnings if GCC is used.
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic")
endif()

# Use 'RelWithDebInfo' as default CMAKE_BUILD_TYPE.
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo)
endif()

set(Boost_USE_MULTITHREADED TRUE)
find_package(Boost REQUIRED filesystem)
find_package(Boost COMPONENTS unit_test_framework)
find_package(Minuit2 REQUIRED)

add_subdirectory(src ${CMAKE_BUILD_DIR}/..)

if(Boost_UNIT_TEST_FRAMEWORK_FOUND)
    add_subdirectory(test)
endif()
