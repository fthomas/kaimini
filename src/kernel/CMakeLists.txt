execute_process(
  COMMAND ${CMAKE_CXX_COMPILER} --version
  COMMAND head -n1
  OUTPUT_VARIABLE CXX_COMPILER_ID
  OUTPUT_STRIP_TRAILING_WHITESPACE)

execute_process(
  COMMAND date "+%s"
  OUTPUT_VARIABLE BUILD_TIMESTAMP
  OUTPUT_STRIP_TRAILING_WHITESPACE)

configure_file(Kaimini.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Kaimini.h)
