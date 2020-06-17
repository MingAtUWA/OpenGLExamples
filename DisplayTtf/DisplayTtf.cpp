#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "DisplayTtf.h"

DisplayTtf::DisplayTtf() :
    vao(0), vbo(0)
{

}

int DisplayTtf::init()
{
    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // model data
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER,
        sizeof(GLfloat) * 6 * 4,
        nullptr,
        GL_DYNAMIC_DRAW
        );
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    // shader
    shader.create("../../Shaders/display_ttf.vert",
                  "../../Shaders/display_ttf.frag");
    shader.use();
    glm::mat4 proj_mat = glm::ortho(0.0f, float(width), 0.0f, float(height));
    shader.set_uniform("projection", proj_mat);
    shader.set_uniform("text", 0);

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return -1;
    }

    FT_Face face;
    if (FT_New_Face(ft, "../../Assets/consola.ttf", 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return -1;
    }

    FT_Set_Pixel_Sizes(face, 0, 48);

    GLuint texture; // texture for char
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // ?
    // load first 128 characters of ASCII
    for (unsigned char c = 0; c < 128; c++)
    {
        // Load character glyph 
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        
        // store character
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            face->glyph->advance.x
        };
        characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    return 0;
}

void DisplayTtf::render_text(
    const char *text,
    float x_pos,
    float y_pos,
    float scale,
    glm::vec3 &color
    )
{
    shader.set_uniform("textColor", color);

    glBindVertexArray(vao);
    glActiveTexture(GL_TEXTURE0);
    
    size_t text_len = strlen(text);
    float cx_pos, cy_pos, cwd, cht;
    for (size_t i = 0; i < text_len; ++i)
    {
        Character &ch = characters[text[i]];

        cx_pos = x_pos + ch.bearing.x * scale;
        cy_pos = y_pos - (ch.size.y - ch.bearing.y) * scale;

        cwd = float(ch.size.x) * scale;
        cht = float(ch.size.y) * scale;

        float vertices[6][4] = {
            { cx_pos,       cy_pos + cht, 0.0f, 0.0f },
            { cx_pos,       cy_pos,       0.0f, 1.0f },
            { cx_pos + cwd, cy_pos,       1.0f, 1.0f },
            { cx_pos,       cy_pos + cht, 0.0f, 0.0f },
            { cx_pos + cwd, cy_pos,       1.0f, 1.0f },
            { cx_pos + cwd, cy_pos + cht, 1.0f, 0.0f }
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData

        glBindTexture(GL_TEXTURE_2D, ch.tex_id);

        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // advance for the next glyph
        // note that advance is number of 1/64 pixels
        // bitshift by 6 to get value in pixels
        // 2^6 = 64 
        // divide amount of 1/64th pixels by 64 to get amount of pixels
        x_pos += (ch.advance >> 6) * scale;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

int DisplayTtf::paint()
{
    glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    render_text("This is sample text", 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));
    render_text("(C) LearnOpenGL.com", 540.0f, 570.0f, 0.5f, glm::vec3(0.3, 0.7f, 0.9f));
    
    return 0;
}

void DisplayTtf::destroy()
{
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
}
