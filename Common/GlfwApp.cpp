#include <iostream>

#include "GlfwApp.h"

GlfwApp* GlfwApp::cur_app = nullptr;

static void glfw_error_callback(int id, const char* description)
{
    GlfwApp::get_cur_app()->error(id, description);
}

static void glfw_resize_callback(GLFWwindow* win, int wd, int ht)
{
    (void)win;
    GlfwApp::get_cur_app()->resize(wd, ht);
}

static void glfw_mouse_move_callback(GLFWwindow* win, double xpos, double ypos)
{
    (void)win;
    GlfwApp::get_cur_app()->mouse_move(xpos, ypos);
}

void glfw_mouse_scroll_callback(GLFWwindow* win, double xoffset, double yoffset)
{
    (void)win;
    GlfwApp::get_cur_app()->mouse_scroll(yoffset);
}


GlfwApp::GlfwApp() :
    win_name("OpenGL application with glfw"),
    width(0), height(0),
    window(nullptr)
{

}

GlfwApp::~GlfwApp()
{

}

int GlfwApp::init_app()
{
    window = glfwCreateWindow(
                width, height,
                win_name.c_str(), 
                nullptr, nullptr
                );
    if (!window)
    {
        std::cout << "Glfw can't create window.\n";
        return -1;
    }

    set_cur_app();
    glfwMakeContextCurrent(window);

    // error callback
    glfwSetErrorCallback(glfw_error_callback);
    // resize callback
    glfwSetFramebufferSizeCallback(window, glfw_resize_callback);
    // mouse move callback
    glfwSetCursorPosCallback(window, glfw_mouse_move_callback);
    // mouse scroll callback
    glfwSetScrollCallback(window, glfw_mouse_scroll_callback);

    // init data
    return init();
}

void GlfwApp::destroy_app()
{
    destory();

    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }
    glfwTerminate();
}

int GlfwApp::run(int wd, int ht)
{
    // init
    width = wd;
    height = ht;
    init_app();

    // main loop
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        glfwPollEvents();
        
        process_keyboard_input();

        paint();

        glfwSwapBuffers(window);
    }

    // exit
    destroy_app();

    return 0;
}

int GlfwApp::resize(int wd, int ht)
{
    return 0;
}

void GlfwApp::error(int id, const char* description)
{
    std::cout << "Glfw Error " << id << ":\n  " << description << "\n";
}

void GlfwApp::mouse_move(double xpos, double ypos)
{

}

void GlfwApp::mouse_scroll(double offset)
{

}

void GlfwApp::process_keyboard_input()
{
    if (key_is_pressed(GLFW_KEY_ESCAPE))
        close_window();
}
