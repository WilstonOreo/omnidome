find_package(Qt5 COMPONENTS Core Gui REQUIRED)

set(SOURCES
    main.cpp
    projectionwindow.h
    projectionwindow.cpp
    )

add_executable(test_glwindow ${SOURCES})

target_include_directories(test_glwindow
    PUBLIC
        ../../lib/include
)

target_link_libraries(test_glwindow
    PUBLIC
        Qt5::Core
        Qt5::Gui
        omni
)
