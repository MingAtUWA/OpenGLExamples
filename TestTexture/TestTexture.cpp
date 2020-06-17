#include "stb_image.h"

#include "TestTexture.h"

TestTexture::TestTexture() :
    tex_dim(two_d),
    vao(0), vbo(0), ebo(0),
    texture1d(0), texture2d1(0), texture2d2(0)
{

}

int TestTexture::init()
{
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    GLfloat vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    GLuint indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        sizeof(vertices),
        vertices,
        GL_STATIC_DRAW
    );

    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(indices),
        indices,
        GL_STATIC_DRAW
    );

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    if (tex_dim == one_d)
    {
        // 1d texture data
        unsigned char color_map_data[256][3];
        for (size_t i = 0; i < 256; ++i)
        {
            color_map_data[i][0] = unsigned char(255 - i);
            color_map_data[i][1] = 0;
            color_map_data[i][2] = unsigned char(i);
        }

        glGenTextures(1, &texture1d);
        glBindTexture(GL_TEXTURE_1D, texture1d);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexImage1D(GL_TEXTURE_1D, 0, GL_RGB, 256, 0, GL_RGB, GL_UNSIGNED_BYTE, color_map_data);
        glGenerateMipmap(GL_TEXTURE_1D);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_1D, texture1d);

        shader_1d.create("../../Shaders/test_texture1D.vert",
                         "../../Shaders/test_texture1D.frag");
        shader_1d.use();
        shader_1d.set_uniform("color_map", 0);
    }
    else if (tex_dim == two_d)
    {
        // ===================== 2D Texture =====================
        // texture 1
        glGenTextures(1, &texture2d1);
        glBindTexture(GL_TEXTURE_2D, texture2d1);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char* data = stbi_load("../../Assets/test_textures_figures/wall.jpg", &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load 2d texture.\n";
        stbi_image_free(data);

        // texture 2
        glGenTextures(1, &texture2d2);
        glBindTexture(GL_TEXTURE_2D, texture2d2);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image
        data = stbi_load("../../Assets/test_textures_figures/awesomeface.png", &width, &height, &nrChannels, 0);
        if (data)
        {
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
            std::cout << "Failed to load 2d texture.\n";
        stbi_image_free(data);

        glActiveTexture(GL_TEXTURE0); // 0
        glBindTexture(GL_TEXTURE_2D, texture2d1);
        glActiveTexture(GL_TEXTURE1); // 1
        glBindTexture(GL_TEXTURE_2D, texture2d2);

        shader_2d.create("../../Shaders/test_texture2D.vert",
                         "../../Shaders/test_texture2D.frag");
        shader_2d.use();
        shader_2d.set_uniform("texture1", 0);
        shader_2d.set_uniform("texture2", 1);
    }

    return 0;
}

int TestTexture::paint()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (tex_dim == one_d)
    {
        shader_1d.use();
    }
    else if (tex_dim == two_d)
    {
        shader_2d.use();
    }
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    return 0;
}

void TestTexture::destroy()
{
    if (ebo)
    {
        glDeleteBuffers(1, &ebo);
        ebo = 0;
    }
    if (vbo)
    {
        glDeleteBuffers(1, &vbo);
        vbo = 0;
    }
    if (vao)
    {
        glDeleteVertexArrays(1, &vao);
        vao = 0;
    }
    if (texture1d)
    {
        glDeleteTextures(1, &texture1d);
        texture1d = 0;
    }
    if (texture2d1)
    {
        glDeleteTextures(1, &texture2d1);
        texture2d1 = 0;
    }
    if (texture2d2)
    {
        glDeleteTextures(1, &texture2d2);
        texture2d2 = 0;
    }
}