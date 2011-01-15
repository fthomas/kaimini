cmake_minimum_required(VERSION 2.6)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)

project(Kaimini C CXX)
string(TOLOWER ${CMAKE_PROJECT_NAME} PROJECT_NAME_LOWER)

set(Kaimini_VERSION_MAJOR "0")
set(Kaimini_VERSION_MINOR "2")
set(Kaimini_VERSION_PATCH "6")
set(Kaimini_VERSION "${Kaimini_VERSION_MAJOR}.${Kaimini_VERSION_MINOR}.${Kaimini_VERSION_PATCH}")

# Enable more warnings if g++, clang or icpc is used.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER MATCHES clang)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pedantic -Wall -Wextra")
elseif(CMAKE_CXX_COMPILER MATCHES icpc)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -strict-ansi -Wall -Wcheck")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -wd383 -wd444 -wd981")
endif()

find_package(Boost REQUIRED COMPONENTS filesystem program_options)
find_package(Boost COMPONENTS unit_test_framework)
find_package(Minuit2 REQUIRED)
find_package(OpenMP)

find_package(Doxygen)
find_package(Git)
find_package(LATEX)

enable_testing()
set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_COPY ${CMAKE_COMMAND} -E copy)

add_subdirectory(doc)
add_subdirectory(l10n)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(tools)

# CPack settings:
set(CPACK_PACKAGE_VENDOR "Frank S. Thomas")
set(CPACK_PACKAGE_VERSION_MAJOR ${Kaimini_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${Kaimini_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${Kaimini_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION       ${Kaimini_VERSION})
set(CPACK_RESOURCE_FILE_LICENSE ${CMAKE_SOURCE_DIR}/COPYING)
set(CPACK_RESOURCE_FILE_README  ${CMAKE_SOURCE_DIR}/README.rst)
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${PROJECT_NAME_LOWER}-${Kaimini_VERSION}")
set(CPACK_SOURCE_GENERATOR TGZ ZIP)
include(CPack)

set(ARCHIVE_FILE_NAME ${CPACK_SOURCE_PACKAGE_FILE_NAME})
set(DEBIAN_ORIG_ARCHIVE "${PROJECT_NAME_LOWER}_${Kaimini_VERSION}.orig")
if(GIT_FOUND)
    add_custom_target(dist
      COMMAND ${GIT_EXECUTABLE} archive
        --format=tar --prefix=${ARCHIVE_FILE_NAME}/ HEAD |
        gzip -9 > ${CMAKE_BINARY_DIR}/${ARCHIVE_FILE_NAME}.tar.gz
      COMMAND ${GIT_EXECUTABLE} archive
        --format=zip --prefix=${ARCHIVE_FILE_NAME}/
        -9 -o ${CMAKE_BINARY_DIR}/${ARCHIVE_FILE_NAME}.zip HEAD
      COMMAND ${CMAKE_COPY}
        ${CMAKE_BINARY_DIR}/${ARCHIVE_FILE_NAME}.tar.gz
        ${CMAKE_BINARY_DIR}/${DEBIAN_ORIG_ARCHIVE}.tar.gz
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
endif()
