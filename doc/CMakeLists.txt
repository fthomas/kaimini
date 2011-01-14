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
    add_custom_command(OUTPUT kaimini.1
      COMMAND ${DOCBOOK2X} ${CMAKE_CURRENT_SOURCE_DIR}/kaimini.xml
      DEPENDS kaimini.xml)
    add_custom_target(manpage ALL DEPENDS kaimini.1)

    install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kaimini.1
      DESTINATION share/man/man1)
endif()
