#ifndef __Mesh_GL_Buffer_h__
#define __Mesh_GL_Buffer_h__

#include <string>
#include <vector>

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "OpenGLShaderUtilities.h"

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 tex_coord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    std::string filename;
    std::string type;
    GLuint id;
};

class MeshGLBuffer
{
protected:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;
    
    GLuint vao, vbo, ebo;

public:
    MeshGLBuffer() {}
    ~MeshGLBuffer();

    inline std::vector<Vertex>& get_vertices() { return vertices; }
    inline std::vector<GLuint>& get_indices() { return indices; }
    inline std::vector<Texture>& get_textures() { return textures; }

    void setup_gl_buffer();
    
    void draw(OpenGLShaderProgram& shader);
    void clear();
};

#endif