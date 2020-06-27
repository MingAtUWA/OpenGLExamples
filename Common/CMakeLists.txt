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
    GlfwApp.h GlfwApp.cpp
    OpenGLShaderUtilities.h OpenGLShaderUtilities.cpp
    CirclesGLBuffer.h CirclesGLBuffer.cpp circle_mesh_data.h
    )

set(COMMON_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    Common PUBLIC
    # External
    ${OPENGL_INCLUDE_DIR}
    )
