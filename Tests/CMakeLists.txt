#===================================
#
# Unit tests
#
#===================================
add_executable(
    Tests
    #
    TestsMain.h TestsMain.cpp
    test_imgui.cpp
    test_texture.cpp
    test_load_obj_model.cpp
    test_display_ttf.cpp
    test_pds_result_view.cpp
    test_random_point_queue.cpp
    )

target_include_directories(
    Tests PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    ${DISPLAYTTF_INCLUDE_DIR}
    ${IMGUIAPP_INCLUDE_DIR}
    ${TESTTEXTURE_INCLUDE_DIR}
    ${LOADOBJFILE_INCLUDE_DIR}
    ${POISSONDISKSAMPLING_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    )

target_link_libraries(
    Tests
    # Internal
    Common
    ImGUIApp
    TestTexture
    LoadObjFile
    DisplayTtf
    PoissonDiskSampling
    # External
    ${GLFW_LIBRARIES}
    )

target_link_libraries(
    Tests
    debug ${ASSSIMP_DEBUG_LIBRARIES}
    optimized ${ASSSIMP_RELEASE_LIBRARIES}
    )
