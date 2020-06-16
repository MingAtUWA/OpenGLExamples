#===================================
#
# Display ttf characters
#
#===================================
add_library(
    DisplayTtf STATIC
    #
    DisplayTtf.h DisplayTtf.cpp
    )

set(DISPLAYTTF_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    DisplayTtf PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    )
