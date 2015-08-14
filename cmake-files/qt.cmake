# Qt Setup 
include(CMakeParseArguments)

# qt5_wrap_ui(outfiles inputfile ... )
function(QT5_GENERATE_UI outfiles )
    set(options)
    set(oneValueArgs)
    set(multiValueArgs OPTIONS)

    cmake_parse_arguments(_WRAP_UI "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    set(ui_files ${_WRAP_UI_UNPARSED_ARGUMENTS})
    set(ui_options ${_WRAP_UI_OPTIONS})

    foreach(it ${ui_files})
        get_filename_component(outfile ${it} NAME_WE)
        get_filename_component(infile ${it} ABSOLUTE)
        set(outfile ${CMAKE_SOURCE_DIR}/moc/ui_${outfile}.h)
        
        add_custom_command(OUTPUT ${outfile}
          COMMAND ${Qt5Widgets_UIC_EXECUTABLE}
          ARGS ${ui_options} -o ${outfile} ${infile}
         MAIN_DEPENDENCY ${infile} VERBATIM)
       #execute_process(COMMAND ${QT_UIC_EXECUTABLE} ${ui_options} -o ${outfile} ${infile}
       #  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
 
        list(APPEND ${outfiles} ${outfile})
    endforeach()
    set(${outfiles} ${${outfiles}} PARENT_SCOPE)
endfunction()


MACRO(find_qt5_component COMPONENT_NAME) 

  set(Qt5${COMPONENT_NAME}_DIR ${QT_LIBRARY_DIR}/cmake/Qt5${COMPONENT_NAME} )
  find_package(Qt5${COMPONENT_NAME} REQUIRED)

  set(Qt5_LIBRARIES ${Qt5_LIBRARIES} Qt5::${COMPONENT_NAME})
  
  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    include_directories(${Qt5${COMPONENT_NAME}_INCLUDE_DIRS})
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    include_directories(${QT_INCLUDE_DIR}/Qt${COMPONENT_NAME})
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  
  if (${COMPONENT_NAME} MATCHES "Widgets")
    set(Qt5${COMPONENT_NAME}_UIC_EXECUTABLE ${QT5_LOCATION}/bin/uic)
  endif (${COMPONENT_NAME} MATCHES "Widgets")

ENDMACRO(find_qt5_component COMPONENT_NAME)

# Qt5 Setup
MACRO(setup_qt MAJOR_VERSION MINOR_VERSION FOLDER)
  SET(_moc    ${CMAKE_SOURCE_DIR}/moc )
  set(QT_MAJOR_VERSION ${MAJOR_VERSION})
  set(QT_MINOR_VERSION ${MINOR_VERSION})
  set(QT_VERSION "${MAJOR_VERSION}.${MINOR_VERSION}")
  MESSAGE(STATUS "Using Qt ${QT_VERSION}")

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(QT5_LOCATION ${FOLDER}/${QT_VERSION}/clang_64)
    include_directories(${FOLDER}/${QT_VERSION}/Src/qtbase/include)
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin") 

  IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(QT5_LOCATION ${FOLDER}/${QT_VERSION}/gcc_64)
  ENDIF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")

  set(QT_QMAKE_EXECUTABLE ${QT5_LOCATION}/bin/qmake)
  set(QT_MOC_EXECUTABLE ${QT5_LOCATION}/bin/moc)
  set(QT_RCC_EXECUTABLE ${QT5_LOCATION}/bin/rcc)
  set(QT_UIC_EXECUTABLE ${QT5_LOCATION}/bin/uic)
  set(QT_INCLUDE_DIR ${QT5_LOCATION}/include)
  set(QT_LIBRARY_DIR ${QT5_LOCATION}/lib)

  find_qt5_component(Core)
  find_qt5_component(Gui)
  find_qt5_component(Widgets)
  find_qt5_component(OpenGL)

  # The Qt5Widgets_INCLUDES also includes the include directories for
  # dependencies QtCore and QtGui
  include_directories(${QT_INCLUDE_DIR} ${CMAKE_SOURCE_DIR}/moc)

  # We need add -DQT_WIDGETS_LIB when using QtWidgets in Qt 5.
  add_definitions(${Qt5Widgets_DEFINITIONS})
ENDMACRO(setup_qt MAJOR_VERSION MINOR_VERSION)


IF(${CMAKE_BUILD_TYPE} MATCHES "Debug")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Debug")

IF(${CMAKE_BUILD_TYPE} MATCHES "Release")
  ADD_DEFINITIONS("-DQT_NO_DEBUG_OUTPUT -DQT_NO_WARNING_OUTPUT")
ENDIF(${CMAKE_BUILD_TYPE} MATCHES "Release")


