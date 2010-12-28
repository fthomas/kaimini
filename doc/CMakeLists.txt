add_subdirectory(manual)

if(DOXYGEN_FOUND)
    configure_file(Doxyfile.in Doxyfile)
    add_custom_target(doxygen
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
      SOURCES Doxyfile.in)
endif()

find_program(DOCBOOK2X docbook2x-man)
if(DOCBOOK2X)
    add_custom_target(manpage
      COMMAND ${DOCBOOK2X} ${CMAKE_CURRENT_SOURCE_DIR}/kaimini.xml
      SOURCES kaimini.xml)
endif()
