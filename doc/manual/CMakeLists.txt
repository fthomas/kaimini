set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CURR_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})

if(PDFLATEX_COMPILER AND BIBTEX_COMPILER)
    configure_file(kaimini.tex . COPYONLY)
    configure_file(kaimini.bib . COPYONLY)
    add_custom_command(OUTPUT ${CURR_BIN_DIR}/kaimini.pdf
      COMMAND ${PDFLATEX_COMPILER} kaimini
      COMMAND ${BIBTEX_COMPILER}   kaimini
      COMMAND ${PDFLATEX_COMPILER} kaimini
      COMMAND ${PDFLATEX_COMPILER} kaimini
      COMMAND ${CMAKE_COPY} kaimini.pdf ${CURR_SRC_DIR}/
      DEPENDS ${CURR_SRC_DIR}/kaimini.tex
              ${CURR_SRC_DIR}/kaimini.bib)

    add_custom_target(manual DEPENDS ${CURR_BIN_DIR}/kaimini.pdf
      SOURCES kaimini.tex kaimini.bib)
endif()
