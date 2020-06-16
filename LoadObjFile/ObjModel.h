#ifndef __Obj_Model_h__
#define __Obj_Model_h__

#include <string>

#include <glad/glad.h> 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <stb_image.h>

#include "OpenGLShaderUtilities.h"
#include "MeshGLBuffer.h"

GLuint load_texture_from_file(
	const char* path,
	const std::string& directory,
	bool gamma = false);

class ObjModel
{
protected:
    std::vector<Texture> textures_loaded;
    std::vector<MeshGLBuffer> meshes;
    std::string directory;
    bool gamma_correction;

public:
    ObjModel() : gamma_correction(false) {}
    ~ObjModel() {}

    // loads a model with supported ASSIMP extensions from file
    // stores resulting meshes in the meshes vector.
    void load_model(const std::string& path, bool gamma = false);

    void draw(OpenGLShaderProgram &shader)
    {
        for (size_t m_id = 0; m_id < meshes.size(); ++m_id)
            meshes[m_id].draw(shader);
    }

protected:
    // processes each individual mesh recursively.
    void process_node(aiNode* node, const aiScene* scene);

    MeshGLBuffer process_mesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture> load_material_textures(aiMaterial* mat, aiTextureType type, const char *typeName);
};

#endif