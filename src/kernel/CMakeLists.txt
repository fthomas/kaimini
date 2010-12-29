execute_process(
  COMMAND ${CMAKE_CXX_COMPILER} --version
  COMMAND head -n1
  COMMAND tr -d '\r\n'
  OUTPUT_VARIABLE CXX_COMPILER_ID)

execute_process(
  COMMAND date "+%s"
  COMMAND tr -d '\r\n'
  OUTPUT_VARIABLE BUILD_TIMESTAMP)

configure_file(Kaimini.h.in ${CMAKE_CURRENT_SOURCE_DIR}/Kaimini.h)
