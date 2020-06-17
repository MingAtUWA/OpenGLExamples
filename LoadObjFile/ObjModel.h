#ifndef __Obj_Model_h__
#define __Obj_Model_h__

#include <string>
#include <unordered_map>

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "OpenGLShaderUtilities.h"
#include "MeshGLBuffer.h"

class ObjModel
{
protected:
    std::vector<MeshGLBuffer> meshes;
    // Assume textures locates in the same directories as obj file
    std::string directory;

    // map texture filename to id
    std::unordered_map<std::string, GLuint> textures_map;

public:
    ObjModel() {}
    ~ObjModel();

    void load_model(const char *model_filename);

    void draw(OpenGLShaderProgram &shader)
    {
        for (size_t m_id = 0; m_id < meshes.size(); ++m_id)
            meshes[m_id].draw(shader);
    }

    // for debug
    void print_info();

protected:
    // processes each mesh node recursively.
    void process_node(aiNode* node, const aiScene* scene);

    void process_mesh(aiMesh* mesh, const aiScene* scene, MeshGLBuffer &buffer);
    void load_material_textures(aiMaterial* mat,
        aiTextureType type, const char *typeName,
        std::vector<Texture>& textures);
};

#endif