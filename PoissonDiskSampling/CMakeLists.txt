#===================================
#
# Poisson disk sampling method
#
#===================================
add_library(
    PoissonDiskSampling STATIC
    #
    PoissonDiskSampling.h PoissonDiskSampling.cpp
    PDSResultView.h PDSResultView.cpp
    pds_utils.h pds_utils.cpp
    BgGrid.h BgGrid.cpp
    RandomPointQueueBase.h
    RandomPointQueueByHash.h RandomPointQueueByHash.cpp
    RandomPointQueueByTree.h RandomPointQueueByTree.cpp
    )

set(POISSONDISKSAMPLING_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    PoissonDiskSampling PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    )

target_link_libraries(
    PoissonDiskSampling PUBLIC
    # Internal
    Common
    )