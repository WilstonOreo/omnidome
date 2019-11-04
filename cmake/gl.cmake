
function(setup_gl)
    if (APPLE)
        find_library(GL_LIBS OpenGL)
        set(${GL_LIBRARIES} ${GL_LIBS} CACHE PARENT_SCOPE)
    endif()
    if (WIN32)
        find_package(OpenGL REQUIRED)
        set(${GL_LIBRARIES} opengl32 CACHE PARENT_SCOPE)
    endif()
endfunction()

setup_gl()
