file(GLOB_RECURSE kaimini_SOURCES
  ${CMAKE_SOURCE_DIR}/src/*.cpp
  ${CMAKE_SOURCE_DIR}/src/*.h)
foreach(ABS_FILE ${kaimini_SOURCES})
    file(RELATIVE_PATH REL_FILE ${CMAKE_SOURCE_DIR} ${ABS_FILE})
    list(APPEND kaimini_SOURCES_REL ${REL_FILE})
endforeach()

find_program(XGETTEXT xgettext)
if(XGETTEXT)
    add_custom_command(OUTPUT kaimini.pot
      COMMAND ${XGETTEXT} --directory=${CMAKE_SOURCE_DIR}/ --output=kaimini.pot
        --c++ --from-code=utf-8 --keyword=_ --boost
        --package-name=${CMAKE_PROJECT_NAME}
        --package-version=${Kaimini_VERSION}
        --msgid-bugs-address=fthomas@physik.uni-wuerzburg.de
        ${kaimini_SOURCES_REL}
      COMMAND ${CMAKE_COPY} kaimini.pot ${CMAKE_CURRENT_SOURCE_DIR}/
      DEPENDS ${kaimini_SOURCES})

    add_custom_target(gettext DEPENDS kaimini.pot)
endif()
