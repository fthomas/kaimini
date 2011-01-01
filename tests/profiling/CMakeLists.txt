set(CURR_SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(CURR_BIN_DIR ${CMAKE_CURRENT_BINARY_DIR})
string(REPLACE "/" "\\/" ESC_BIN_DIR ${CMAKE_BINARY_DIR}/)

function(run_valgrind TOOL BINARY OUTFILE)
    find_program(PROFILER_valg valgrind)
    if(PROFILER_valg)
        get_target_property(BIN_LOC ${BINARY} LOCATION)
        add_custom_command(
          OUTPUT  ${CURR_BIN_DIR}/${OUTFILE}
          COMMAND ${PROFILER_valg} --tool=${TOOL} ${BIN_LOC}
            > ${OUTFILE} 2>&1 || true
          COMMAND sed -i 's/==[0-9]\\+==[ ]*//' ${OUTFILE}
          COMMAND sed -i 's/${ESC_BIN_DIR}//' ${OUTFILE}
          COMMAND ${CMAKE_COPY} ${OUTFILE} ${CURR_SRC_DIR}/
          DEPENDS ${BINARY})
        set(VALG_RESULTS ${VALG_RESULTS};${CURR_BIN_DIR}/${OUTFILE}
          PARENT_SCOPE)
    endif()
endfunction()

run_valgrind(callgrind ut ut-callgrind.txt)
run_valgrind(memcheck  ut ut-memcheck.txt)

add_custom_target(profiles DEPENDS ${VALG_RESULTS})
