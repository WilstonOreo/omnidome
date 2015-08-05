
MACRO(test name)
  ADD_EXECUTABLE(test/${name} src/test/${name}.cpp)
  TARGET_LINK_LIBRARIES(test/${name} ${Boost_LIBRARIES} ${BIGREP_EXTRALIBS})

  SET_TARGET_PROPERTIES(test/${name} PROPERTIES EXCLUDE_FROM_DEFAULT_BUILD_DEBUG True)

ENDMACRO(test name)
