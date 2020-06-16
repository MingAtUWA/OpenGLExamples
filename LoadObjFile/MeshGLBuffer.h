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
    glm::vec2 tex_coords;
    glm::vec3 tangent;
    glm::vec3 bitangent;
};

struct Texture
{
    std::string path;
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
    MeshGLBuffer(std::vector<Vertex>& _vertices,
                 std::vector<GLuint> &_indices,
                 std::vector<Texture> &_textures);
    ~MeshGLBuffer();

    void MeshGLBuffer::draw(OpenGLShaderProgram& shader);

protected:
    void setup_mesh();
};

#endif