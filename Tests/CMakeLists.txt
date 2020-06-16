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
    test_load_obj_model.cpp
    )

target_include_directories(
    Tests PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    ${DISPLAYTTF_INCLUDE_DIR}
    ${IMGUIAPP_INCLUDE_DIR}
    ${LOADOBJFILE_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    )

target_link_libraries(
    Tests
    # Internal
    Common
    DisplayTtf
    ImGUIApp
    LoadObjFile
    # External
    ${GLFW_LIBRARIES}
    )

target_link_libraries(
    Tests
    debug ${ASSSIMP_DEBUG_LIBRARIES}
    optimized ${ASSSIMP_RELEASE_LIBRARIES}
    )
