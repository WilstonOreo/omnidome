
# add a target to generate API documentation with Doxygen
FIND_PACKAGE(Doxygen)

IF(DOXYGEN_FOUND MATCHES YES)
  SET(DOXYGEN_FILE_ABS_PATH ${CMAKE_PROJECT_DOC_DIR}/${PROJECT_NAME}.doxygen)

  if (EXISTS ${DOXYGEN_FILE_ABS_PATH})
    CONFIGURE_FILE(${DOXYGEN_FILE_ABS_PATH} ${CMAKE_PROJECT_DOC_DIR} @ONLY)
    ADD_CUSTOM_TARGET(doc
      ${DOXYGEN_EXECUTABLE} ${DOXYGEN_FILE_ABS_PATH}
      WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen (see ./doc/html/index.html)" VERBATIM
    )
    MESSAGE(STATUS "Doxygen is setup. Use 'make doc' to generate documentation.")
  ELSE()
    MESSAGE(WARNING "Could not setup Doxygen!")
    
  endif()
ENDIF()
