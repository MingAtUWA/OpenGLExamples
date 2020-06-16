#ifndef __Glfw_App_h__
#define __Glfw_App_h__

#include <string>
#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class GlfwApp
{
protected:
	class EnvInitializer
	{
	public:
		EnvInitializer()
		{
			if (!glfwInit())
			{
				std::cout << "Glfw can't initialize.\n";
				return;
			}
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
			if (!gladLoadGL())
			{
				std::cout << "Glad can't load GL functions.\n";
				return;
			}
		}
	} env_initializer;

	std::string win_name;
	int width, height;
	GLFWwindow* window;

	int init_app();
	void destroy_app();

public:
	GlfwApp();
	~GlfwApp();

	inline void set_win_size(int wd, int ht) { width = wd; height = ht; }
	inline void set_win_name(const char* name) { win_name = name; }

	int run(int wd = 800, int ht = 800);

public:
	// function written by user
	virtual int init() = 0;
	virtual int paint() = 0;
	virtual void destory() = 0;
	virtual int resize(int wd, int ht);
	virtual void error(int id, const char* description);
	virtual void mouse_move(double xpos, double ypos);
	virtual void mouse_scroll(double offset);
	virtual void process_keyboard_input();

	// utilities
	// key macros:
	// GLFW_KEY_S
	// GLFW_KEY_A
	// GLFW_KEY_D
	inline bool key_is_pressed(int key) { return glfwGetKey(window, key) == GLFW_PRESS; }
	inline void close_window() { glfwSetWindowShouldClose(window, true); }

private:	
	// current application instance
	static GlfwApp *cur_app;
	inline void set_cur_app() { cur_app = this; }

	// no copy
	GlfwApp(const GlfwApp& other) = delete;
	GlfwApp& operator=(const GlfwApp& other) = delete;

public:
	inline static GlfwApp* get_cur_app() { return cur_app; }
};

#endif