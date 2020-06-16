#===================================
#
# Load object file
#
#===================================
add_library(
    LoadObjFile STATIC
    #
    MeshGLBuffer.h MeshGLBuffer.cpp
    ObjModel.h ObjModel.cpp
    Camera_YawPitch.h Camera_YawPitch.cpp
    LoadObjFile.h LoadObjFile.cpp
    )

set(LOADOBJFILE_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    LoadObjFile PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    ${ASSSIMP_INCLUDE_DIR}
    )

target_link_libraries(
    LoadObjFile PUBLIC
    # Internal
    Common
    # External
    ${ASSSIMP_LIBRARIES}
    )