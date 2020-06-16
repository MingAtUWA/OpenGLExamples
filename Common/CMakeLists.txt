#===================================
#
# Common utilities
#
#===================================
add_library(
    Common STATIC
    #
    glad.c
    stb_image.h std_image.c
    OpenGLShaderUtilities.h OpenGLShaderUtilities.cpp
    GlfwApp.h GlfwApp.cpp
    )

set(COMMON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    Common PUBLIC
    # External
    ${OPENGL_INCLUDE_DIR}
    )
