#include <iostream>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stb_image.h>

#include "LoadObjFile.h"

LoadObjFile::LoadObjFile() :
    camera(glm::vec3(0.0f, 0.0f, 3.0f)),
    last_frame_dtime(0.0f),
    is_first_mouse(true)
{

}

int LoadObjFile::init()
{
    stbi_set_flip_vertically_on_load(true);

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);

    shader.create("../../Shaders/load_obj_file.vert",
                  "../../Shaders/load_obj_file.frag");

    model.load_model("../../Assets/backpack/backpack.obj");
    model.print_info();

    return 0;
}

int LoadObjFile::paint()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glm::mat4 proj_mat = glm::perspective(45.0f, (float)width / (float)height, 0.01f, 1000.0f);
    shader.set_uniform("projection", proj_mat);
    
    glm::mat4 view_mat = camera.get_view_mat();
    shader.set_uniform("view", view_mat);

    glm::mat4 model_mat = glm::mat4(1.0f);
    //model_mat = glm::translate(model_mat, glm::vec3(0.0f, 0.0f, 0.0f));
    //model_mat = glm::scale(model_mat, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set_uniform("model", model_mat);

    model.draw(shader);

    return 0;
}

void LoadObjFile::destroy()
{

}

int LoadObjFile::resize(int wd, int ht)
{
    glViewport(0, 0, wd, ht);
    return 0;
}

void LoadObjFile::mouse_move(double xpos, double ypos)
{
    if (is_first_mouse)
    {
        last_xpos = xpos;
        last_ypos = ypos;
        is_first_mouse = false;
    }

    float xoffset = xpos - last_xpos;
    float yoffset = last_ypos - ypos;

    last_xpos = xpos;
    last_ypos = ypos;

    camera.move_cam_dir(xoffset, yoffset);
}

void LoadObjFile::process_keyboard_input()
{
    float cur_frame_time = glfwGetTime();
    float dtime = cur_frame_time - last_frame_dtime;
    last_frame_dtime = cur_frame_time;

    if (key_is_pressed(GLFW_KEY_W))
        camera.move_cam_pos(Camera_YawPitch::forward, dtime);
    if (key_is_pressed(GLFW_KEY_S))
        camera.move_cam_pos(Camera_YawPitch::backward, dtime);
    if (key_is_pressed(GLFW_KEY_A))
        camera.move_cam_pos(Camera_YawPitch::left, dtime);
    if (key_is_pressed(GLFW_KEY_D))
        camera.move_cam_pos(Camera_YawPitch::right, dtime);

    GlfwApp::process_keyboard_input();
}
