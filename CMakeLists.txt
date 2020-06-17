cmake_minimum_required(VERSION 3.15)

#===================================
#
# OpenGL Example Codes Project
#
#===================================
project(OpenGLExamples)

set(VENDORS_DIR "D:/Vendors")

########### External library headers ############
# GLAD
set(GLAD_INCLUDE_DIR "${VENDORS_DIR}/GLAD/include/")
# GLFW
set(GLFW_INCLUDE_DIR "${VENDORS_DIR}/GLFW/include/")
set(GLFW_LIBRARIES_DIR "${VENDORS_DIR}/GLFW/lib/")
find_library(GLFW_LIBRARIES
    NAMES glfw3
    PATHS ${GLFW_LIBRARIES_DIR}
    )
# glm
set(GLM_INCLUDE_DIR "${VENDORS_DIR}/glm/")
# OpenGL
set(OPENGL_INCLUDE_DIR
    ${GLAD_INCLUDE_DIR}
    ${GLFW_INCLUDE_DIR}
    ${GLM_INCLUDE_DIR}
    )

# Assimp
set(ASSSIMP_INCLUDE_DIR "${VENDORS_DIR}/Assimp-5.0.1/include")
# release
set(ASSSIMP_RELEASE_LIBRARIES_DIR "${VENDORS_DIR}/Assimp-5.0.1/lib")
find_library(ASSSIMP_LIBRARIES_ASSIMP
    NAMES assimp-vc142-mt
    PATHS ${ASSSIMP_RELEASE_LIBRARIES_DIR}
    )
find_library(ASSSIMP_LIBRARIES_IRRXML
    NAMES IrrXML
    PATHS ${ASSSIMP_RELEASE_LIBRARIES_DIR}
    )
find_library(ASSSIMP_LIBRARIES_ZLIBSTATIC
    NAMES zlibstatic
    PATHS ${ASSSIMP_RELEASE_LIBRARIES_DIR}
    )
set(ASSSIMP_RELEASE_LIBRARIES "")
list(APPEND ASSSIMP_RELEASE_LIBRARIES
    ${ASSSIMP_LIBRARIES_ASSIMP}
    ${ASSSIMP_LIBRARIES_IRRXML}
    ${ASSSIMP_LIBRARIES_ZLIBSTATIC}
    )
unset(ASSSIMP_RELEASE_LIBRARIES_DIR CACHE) # need CACHE?
unset(ASSSIMP_LIBRARIES_ASSIMP CACHE)
unset(ASSSIMP_LIBRARIES_IRRXML CACHE)
unset(ASSSIMP_LIBRARIES_ZLIBSTATIC CACHE)
# debug
set(ASSSIMP_DEBUG_LIBRARIES_DIR "${VENDORS_DIR}/Assimp-5.0.1/lib_debug")
find_library(ASSSIMP_LIBRARIES_ASSIMP
    NAMES assimp-vc142-mtd
    PATHS ${ASSSIMP_DEBUG_LIBRARIES_DIR}
    )
find_library(ASSSIMP_LIBRARIES_IRRXML
    NAMES IrrXMLd
    PATHS ${ASSSIMP_DEBUG_LIBRARIES_DIR}
    )
find_library(ASSSIMP_LIBRARIES_ZLIBSTATIC
    NAMES zlibstaticd
    PATHS ${ASSSIMP_DEBUG_LIBRARIES_DIR}
    )
set(ASSSIMP_DEBUG_LIBRARIES "")
list(APPEND ASSSIMP_DEBUG_LIBRARIES
    ${ASSSIMP_LIBRARIES_ASSIMP}
    ${ASSSIMP_LIBRARIES_IRRXML}
    ${ASSSIMP_LIBRARIES_ZLIBSTATIC}
    )
unset(ASSSIMP_DEBUG_LIBRARIES_DIR CACHE)
unset(ASSSIMP_LIBRARIES_ASSIMP CACHE)
unset(ASSSIMP_LIBRARIES_IRRXML CACHE)
unset(ASSSIMP_LIBRARIES_ZLIBSTATIC CACHE)


################ Example projects ###############
# Common
# depend on
#   glad (in external folder)
#   glfw (in external folder)
add_subdirectory(Common/)

# ImGUIApp
# depend on
#   ImGUI
add_subdirectory(ImGUIApp/)

# TestTexture
#   stb_image
add_subdirectory(TestTexture/)

# LoadObjFile
# depend on
#   Assimp (in external folder)
#   stb_image
add_subdirectory(LoadObjFile/)

# Display ttf character
add_subdirectory(DisplayTtf/)

# Tests
add_subdirectory(Tests/)
