#ifndef __ImGUI_App_h__
#define __ImGUI_App_h__

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "GlfwApp.h"

class ImGUIApp : public GlfwApp
{
protected:
	bool show_demo_window;
	bool show_another_window;
	ImVec4 clear_color;

public:
	int init() override;
	int paint() override;
	void destroy() override;
};

#endif