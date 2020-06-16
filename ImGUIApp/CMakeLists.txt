#===================================
#
# App using ImGUI
#
#===================================
add_library(
    ImGUIApp STATIC
    # main class
    ImGUIApp.h ImGUIApp.cpp
    # ImGui lib
    imgui.h
    imconfig.h
    imstb_rectpack.h
    imstb_textedit.h
    imgui_internal.h
    imstb_truetype.h
    imgui_impl_opengl3.h
    imgui_impl_glfw.h
    imgui.cpp
    imgui_draw.cpp
    imgui_widgets.cpp
    imgui_demo.cpp
    imgui_impl_opengl3.cpp
    imgui_impl_glfw.cpp
    )

set(IMGUIAPP_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}" CACHE STRING INTERNAL FORCE)

target_include_directories(
    ImGUIApp PUBLIC
    # Internal
    ${COMMON_INCLUDE_DIR}
    # External
    ${OPENGL_INCLUDE_DIR}
    )
