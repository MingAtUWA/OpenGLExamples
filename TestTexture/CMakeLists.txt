#===================================
#
# Test texture project
#
#===================================
add_library(
    TestTexture STATIC
    #
    TestTexture.h TestTexture.cpp
    )

set(TESTTEXTURE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    TestTexture PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    )

target_link_libraries(
    TestTexture PUBLIC
    # Internal
    Common
    )